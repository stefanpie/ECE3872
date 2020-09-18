#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void delay(int milliseconds)
{
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

int analogRead(int pin){
    printf("Simulating analog read on analog pin %d\n", pin);
    int analogValue = rand() % (1023 - 0 + 1) + 0;
    return analogValue;
}

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void tone(int pin, int frequency){
    printf("Simulating starting PWM tone output on PWM out pin %d with frequency %d\n", pin, frequency);
}

void noTone(int pin){
    printf("Simulating stopping PWM tone output on PWM out pin %d\n", pin);
}