#include <stdio.h>

#include "sim_driver.h"

int mode = 1;

int step_time = 1;
int step_gate = 0.5;
int loop_length = 4;
int current_step = 0;
int looping = 0;
int current_step_pitch = 0;

int recording[16];
int scale_a_minor[12] = {440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 840};

int analog_note_inputs[4] = {A0, A1, A2, A3};
int audio_signal_output = 3;

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
    }
    
    // LIVE SEQUENCE
    if (mode == 1)
    {
        printf("Mode: LIVE SEQUENCE");
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

            printf("\n");

            current_step++;
            if (current_step >= loop_length)
            {
                current_step = 0;
            }
        }
    }
    else
    {
        mode = 0;
    }
}

int main()
{
    // printf() displays the string inside quotation
    printf("Starting Simulation...");
    printf("Call Setup Function...");
    setup();
    printf("Call Loop Function...");
    while (1)
    {
        loop();
    }
    return 0;
}