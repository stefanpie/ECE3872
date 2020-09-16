#include <stdio.h>

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

void setup(){

    printf("Steup Function")
    // Serial.begin(115200);
    // Replace with Serial calls with printf calls 
}

void loop(){

}



int main()
{
    // printf() displays the string inside quotation
    printf("Starting Simulation...");
    printf("Call Setup Function...");
    setup();
    printf("Call Loop Function...");
    while(1){
        loop();
    }
    return 0;
}