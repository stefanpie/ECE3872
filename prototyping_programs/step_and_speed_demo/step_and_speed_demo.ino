#include <Stepper.h>
#define STEPS 200
//Stepper stepper(STEPS, 4, 5, 6, 7);
Stepper stepper_step(STEPS, 31, 33, 35, 37);
Stepper stepper_pitch(STEPS, 23, 25, 27, 29);

int current_pitch_position = 0;
int note_positions[8] = {0,25,50,75,100,125,150,175};
int position_diff = 0;

unsigned long StartTime;
unsigned long StopTime;
unsigned long ElapsedTime;

int wait_time;

int mode = 1;

double step_time = 1;
int step_gate = 0.5;
int loop_length = 8;
int current_step = 0;
int looping = 0;
int current_step_pitch = 0;

int recording[16];
//int scale_a_minor[12] = {440,466,494,523,554,587,622,659,698,740,784,840};
int scale_a_minor[8] = {220,247,262,294,330,349,392,440};


int analog_note_inputs[8] = {A0,A1,A2,A3,A4,A5,A6,A7};
int audio_signal_output = 12;

int sequence_speed_input = A15;
double sequence_speed_min_bpm = 70;
double sequence_speed_max_bpm = 200;
double current_speed = 0;



void setup() {
  Serial.begin(115200);
  Serial.println("Live Sequence Function Demo");
  stepper_step.setSpeed(120);
  stepper_pitch.setSpeed(120);
}


void loop() {


  
//    Serial.print("Step: ");
//    Serial.print(current_step);
//    Serial.print("\n");
    
    current_step_pitch = map(analogRead(analog_note_inputs[current_step]), 0, 1023, 7, 0);
//    Serial.print("Pitch: ");
//    Serial.print(current_step_pitch);
//    Serial.print("\n");

    current_speed = double(analogRead(sequence_speed_input))/1023 * (sequence_speed_max_bpm-sequence_speed_min_bpm) + sequence_speed_min_bpm;
//    Serial.print("Speed: ");
//    Serial.print(current_speed);
//    Serial.print("\n");
    step_time = 60 / current_speed;
//
//    Serial.print("Step Time: ");
//    Serial.print(step_time);
//    Serial.print("\n");


//    Serial.print("Current Position of Pitch Servo: ");
//    Serial.print(current_pitch_position);
//    Serial.print("\n");
//
//    Serial.print("Delay Pitch Servo: ");
//    Serial.print(ElapsedTime);
//    Serial.print("\n");


    tone(audio_signal_output, scale_a_minor[current_step_pitch]);


    position_diff = note_positions[current_step_pitch] - current_pitch_position;
    if(position_diff > 100) {position_diff = position_diff-200;}
    if(position_diff < -100) {position_diff = position_diff+200;}

    StartTime = millis();
    stepper_pitch.step(position_diff);
    StopTime = millis();
    ElapsedTime = StopTime - StartTime;
    
    current_pitch_position = (current_pitch_position + position_diff) % 200;

    delay( max((step_time*1000) - ElapsedTime, 0) );

    
//    delay(step_time*1000);
    noTone(audio_signal_output);

    
    stepper_step.step(25);


    
    
//    delay(step_time*1000);
    
    
    current_step++;
    if(current_step>=loop_length){
      current_step=0;
    }
  
}
