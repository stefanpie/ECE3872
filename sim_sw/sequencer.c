#include <stdio.h>

#include "sim_driver.h"

int mode = 0;

int step_time = 1;
int step_gate = 0.5;
int loop_length = 4;
int current_step = 0;
int looping = 0;
int current_step_pitch = 0;

#define recording_size 16
int recording[recording_size] = {-1};

int scale_a_minor[12] = {440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 840};
int analog_note_inputs[4] = {A0, A1, A2, A3};
int audio_signal_output = 3;

char input;

int find(int *a, int n, int key)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (a[i] == key)
        {
            return i;
        }
    }
    return -1;
}

char get_input()
{
    if (kbhit())
    {
        char c = getch();
        printf("Current Input: %c\n", c);
        return c;
    }
    else
    {
        return 0;
    }
}

void setup()
{

    // Serial.begin(115200);
    // Replace with Serial calls with printf calls for sim
    printf("Starting Serial Connection...\n");

}

void loop()
{
    // IDLE
    if (mode == 0)
    {
        printf("Mode: IDLE\n");

        // Check inputs for state transitions
        input = get_input();
        switch (input)
        {
        case 'l':
            printf("Live Pressed\n");
            printf("Going to LIVE SEQUENCE state\n");
            mode = 1;
            break;
        case 'p':
            printf("Play Pressed\n");
            printf("Going to RECORD PLAYBACK state\n");
            mode = 3;
            break;
        case 'r':
            printf("Record Pressed\n");
            printf("Going to RECORD state\n");
            mode = 2;
            break;
        case 'q':
            printf("Reset Pressed\n");
            printf("Going to RESET state\n");
            mode = 4;
            break;
        }
        
    }

    // LIVE SEQUENCE
    if (mode == 1)
    {
        printf("Mode: LIVE SEQUENCE\n");
        looping = 1;
        current_step = 0;

        while (looping)
        {
            printf("Step: ");
            printf("%d", current_step);
            printf("\n");

            current_step_pitch = map(analogRead(analog_note_inputs[current_step]), 0, 1023, 0, 11);
            printf("Pitch: ");
            printf("%d", current_step_pitch);
            printf("\n");

            tone(audio_signal_output, scale_a_minor[current_step_pitch]);
            delay(step_time * 1000);
            noTone(audio_signal_output);

            // Check inputs for state transitions
            input = get_input();
            if(input == 's'){
                printf("Stop Pressed\n");
                printf("Live Play Stopped\n");
                printf("Going to IDLE state\n");
                looping = 0;
                mode = 0;
            }

            current_step++;
            if (current_step >= loop_length)
            {
                current_step = 0;
            }
        }
    }

    // RECORD
    if (mode == 2)
    {
        int starting_index = find(recording, recording_size, -1);
        current_step = starting_index;
        while (current_step < recording_size)
        {
            printf("Recording Step: ");
            printf("%d", current_step);
            printf("\n");

            current_step_pitch = map(analogRead(analog_note_inputs[current_step]), 0, 1023, 0, 11);
            printf("Recording Pitch: ");
            printf("%d", current_step_pitch);
            printf("\n");

            tone(audio_signal_output, scale_a_minor[current_step_pitch]);
            delay(step_time * 1000);
            noTone(audio_signal_output);

            input = get_input();
            if (input == 's')
            {   
                printf("Stop Pressed\n");
                printf("Recording Stopped\n");
                printf("Going to IDLE state\n");
                looping = 0;
                mode = 0;
            }

            current_step++;
        }

        printf("Recording Memory Full\n");
        printf("Going to IDLE State\n");
        mode = 0;
    }

    // RECORD PLAYBACK
    if (mode == 3)
    {
        int ending_index = find(recording, recording_size, -1);
        if (ending_index == 0)
        {
            printf("Recording Memory Empty\n");
            printf("Going to IDLE State\n");
            mode == 0;
        }
        else{
            current_step = 0;
            looping = 1;
            while (looping)
            {
                printf("Record Playback Step: ");
                printf("%d", current_step);
                printf("\n");

                current_step_pitch = recording[current_step];
                printf("Record Playback Pitch: ");
                printf("%d", current_step_pitch);
                printf("\n");

                tone(audio_signal_output, scale_a_minor[current_step_pitch]);
                delay(step_time * 1000);
                noTone(audio_signal_output);

                input = get_input();
                if (input == 's')
                {
                    printf("Stop Pressed\n");
                    printf("Recording Playback Stopped\n");
                    printf("Going to IDLE state\n");
                    looping = 0;
                    mode = 0;
                }

                current_step++;
                if (current_step >= ending_index)
                {
                    current_step = 0;
                }
            }
        }
    }

    // RESET
    if (mode == 4)
    {
        printf("Mode: RESET\n");
        printf("Clearing Recording Memeory\n");
        for (int i = 0; i < recording_size; i++)
        {
            recording[i] = -1;
        }
        printf("Recording Memeory Cleared\n");
        printf("Going to IDLE State\n");
        mode = 0;
    }
}

int main()
{
    // printf() displays the string inside quotation
    printf("Starting Simulation...\n");
    printf("Call Setup Function...\n");
    setup();
    printf("Call Loop Function...\n");
    while (1)
    {
        loop();
    }
    return 0;
}