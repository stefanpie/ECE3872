#define A0 0
#define A1 1
#define A2 2
#define A3 3

void delay(int milliseconds);

int analogRead(int pin);

int map(int x, int in_min, int in_max, int out_min, int out_max);

void tone(int pin, int frequency);
void noTone(int pin);