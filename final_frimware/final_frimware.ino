#include <Stepper.h>
#define STEPS 200
//Stepper stepper(STEPS, 4, 5, 6, 7);
Stepper stepper_step(STEPS, 31, 33, 35, 37);
Stepper stepper_pitch(STEPS, 23, 25, 27, 29);

int current_pitch_position = 0;
int note_positions[8] = {0, 25, 50, 75, 100, 125, 150, 175};
int position_diff = 0;

int current_step_position = 0;


unsigned long StartTime;
unsigned long StopTime;
unsigned long ElapsedTime;


int mode = 0;

double step_time = 1;
int step_gate = 0.5;
int loop_length = 8;
int current_step = 0;
int looping = 0;
int current_step_pitch = 0;

#define recording_size 16
int recording[recording_size] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

int scale_a_minor[8] = {220, 247, 262, 294, 330, 349, 392, 440};


int analog_note_inputs[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
int audio_signal_output = 12;

int sequence_speed_input = A15;
double sequence_speed_min_bpm = 70;
double sequence_speed_max_bpm = 200;
double current_speed = 0;

char input;
#define RECORD_BTN  53
#define PLAY_BTN  51
#define LIVE_BTN  49
#define RESET_BTN  47



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
  if (digitalRead(RECORD_BTN) == LOW)
  { return 'r';
  }
  if (digitalRead(PLAY_BTN) == LOW)
  { return 'p';
  }
  if (digitalRead(LIVE_BTN) == LOW)
  { return 'l';
  }
  if (digitalRead(RESET_BTN) == LOW)
  { return 'q';
  }

  return 0;

}


void enter_beep() {
  tone(audio_signal_output, 500, 500);
  delay(500);
  tone(audio_signal_output, 800, 500);
  delay(500);
  delay(500);
}

void exit_beep() {
  tone(audio_signal_output, 800, 500);
  delay(500);
  tone(audio_signal_output, 500, 500);
  delay(500);
  delay(500);
}

void idle_beep() {
  tone(audio_signal_output, 500, 250);
  delay(300);
  tone(audio_signal_output, 500, 250);
  delay(300);
}



void setup() {
  Serial.begin(115200);
  Serial.println("Live Sequence Function Demo");
  stepper_step.setSpeed(120);
  stepper_pitch.setSpeed(120);
  pinMode(RECORD_BTN, INPUT);
  pinMode(PLAY_BTN, INPUT);
  pinMode(LIVE_BTN, INPUT);
  pinMode(RESET_BTN, INPUT);
}

void loop()
{
  idle_beep();
  delay(1000);

  // IDLE
  if (mode == 0)
  {
    printf("Mode: IDLE\n");

    // Check inputs for state transitions
    input = get_input();
    switch (input)
    {
      case 'l':
        //        printf("Live Pressed\n");
        //        printf("Going to LIVE SEQUENCE state\n");
        mode = 1;
        break;
      case 'p':
        //        printf("Play Pressed\n");
        //        printf("Going to RECORD PLAYBACK state\n");
        mode = 3;
        break;
      case 'r':
        //        printf("Record Pressed\n");
        //        printf("Going to RECORD state\n");
        mode = 2;
        break;
      case 'q':
        //        printf("Reset Pressed\n");
        //        printf("Going to RESET state\n");
        mode = 4;
        break;
      case 'd':
        //        printf("Debug Pressed\n");
        //        printf("Memory: \n");
        //        for (int i = 0; i < recording_size; i++)
        //          printf("%d ", recording[i]);
        //        printf("\n");
        break;
    }
  }

  // LIVE SEQUENCE
  if (mode == 1)
  {
    enter_beep();
    //    printf("Mode: LIVE SEQUENCE\n");
    looping = 1;
    current_step = 0;

    while (looping)
    {


      current_step_pitch = map(analogRead(analog_note_inputs[current_step]), 0, 1023, 7, 0);
      current_speed = double(analogRead(sequence_speed_input)) / 1023 * (sequence_speed_max_bpm - sequence_speed_min_bpm) + sequence_speed_min_bpm;
      step_time = 60 / current_speed;
      tone(audio_signal_output, scale_a_minor[current_step_pitch]);
      position_diff = note_positions[current_step_pitch] - current_pitch_position;
      if (position_diff > 100) {
        position_diff = position_diff - 200;
      }
      if (position_diff < -100) {
        position_diff = position_diff + 200;
      }
      StartTime = millis();
      stepper_pitch.step(position_diff);
      StopTime = millis();
      ElapsedTime = StopTime - StartTime;
      current_pitch_position = (current_pitch_position + position_diff) % 200;
      delay( max((step_time * 1000) - ElapsedTime, 0) );
      noTone(audio_signal_output);
      stepper_step.step(25);
      current_step_position = (current_step_position + 25) % 200;


      input = get_input();
      if (input == 'l')
      {
        stepper_pitch.step(-1 * current_pitch_position);
        current_pitch_position = 0;
        stepper_step.step(-1 * current_step_position);
        current_step_position = 0;
        //        printf("Stop Pressed\n");
        //        printf("Live Play Stopped\n");
        //        printf("Going to IDLE state\n");

        looping = 0;
        mode = 0;
        exit_beep();
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
    enter_beep();
    int starting_index = find(recording, recording_size, -1);
    //    printf("Starting Index: %d\n", starting_index);
    current_step = starting_index;

    if (starting_index == -1) {
      //      printf("Recording Memory Full\n");
      //      printf("Going to IDLE State\n");
      mode = 0;
      exit_beep();
    }
    else
    {
      while (current_step < recording_size)
      {

        current_step_pitch = map(analogRead(analog_note_inputs[current_step]), 0, 1023, 7, 0);
        recording[current_step] = current_step_pitch;
        current_speed = double(analogRead(sequence_speed_input)) / 1023 * (sequence_speed_max_bpm - sequence_speed_min_bpm) + sequence_speed_min_bpm;
        step_time = 60 / current_speed;
        tone(audio_signal_output, scale_a_minor[current_step_pitch]);
        position_diff = note_positions[current_step_pitch] - current_pitch_position;
        if (position_diff > 100) {
          position_diff = position_diff - 200;
        }
        if (position_diff < -100) {
          position_diff = position_diff + 200;
        }
        StartTime = millis();
        stepper_pitch.step(position_diff);
        StopTime = millis();
        ElapsedTime = StopTime - StartTime;
        current_pitch_position = (current_pitch_position + position_diff) % 200;
        delay( max((step_time * 1000) - ElapsedTime, 0) );
        noTone(audio_signal_output);
        stepper_step.step(25);
        current_step_position = (current_step_position + 25) % 200;



        input = get_input();
        if (input == 'r')
        {
          //          printf("Stop Pressed\n");
          //          printf("Recording Stopped\n");
          //          printf("Going to IDLE state\n");
          stepper_pitch.step(-1 * current_pitch_position);
          current_pitch_position = 0;
          stepper_step.step(-1 * current_step_position);
          current_step_position = 0;
          mode = 0;
          break;
          exit_beep();
        };

        current_step = current_step + 1;
      }
      if (current_step == recording_size)
      {
        //        printf("Recording Memory Full\n");
        //        printf("Going to IDLE State\n");
        stepper_pitch.step(-1 * current_pitch_position);
        current_pitch_position = 0;
        stepper_step.step(-1 * current_step_position);
        current_step_position = 0;
        mode = 0;
        exit_beep();
      }
    }
  }

  // RECORD PLAYBACK
  if (mode == 3)
  {
    enter_beep();
    int ending_index = find(recording, recording_size, -1);
    //    printf("Ending Index: %d\n", ending_index);
    if (ending_index == -1)
    {
      ending_index = recording_size - 1;
    }

    if (ending_index == 0)
    {
      //      printf("Recording Memory Empty\n");
      //      printf("Going to IDLE State\n");
      stepper_pitch.step(-1 * current_pitch_position);
      current_pitch_position = 0;
      stepper_step.step(-1 * current_step_position);
      current_step_position = 0;
      mode = 0;
      exit_beep();
    }
    else
    {
      current_step = 0;
      looping = 1;


      while (looping)
      {

        current_step_pitch = recording[current_step];
        recording[current_step] = current_step_pitch;
        current_speed = double(analogRead(sequence_speed_input)) / 1023 * (sequence_speed_max_bpm - sequence_speed_min_bpm) + sequence_speed_min_bpm;
        step_time = 60 / current_speed;
        tone(audio_signal_output, scale_a_minor[current_step_pitch]);
        position_diff = note_positions[current_step_pitch] - current_pitch_position;
        if (position_diff > 100) {
          position_diff = position_diff - 200;
        }
        if (position_diff < -100) {
          position_diff = position_diff + 200;
        }
        StartTime = millis();
        stepper_pitch.step(position_diff);
        StopTime = millis();
        ElapsedTime = StopTime - StartTime;
        current_pitch_position = (current_pitch_position + position_diff) % 200;
        delay( max((step_time * 1000) - ElapsedTime, 0) );
        noTone(audio_signal_output);
        stepper_step.step(25);
        current_step_position = (current_step_position + 25) % 200;



        input = get_input();
        if (input == 'p')
        {
          //          printf("Stop Pressed\n");
          //          printf("Recording Playback Stopped\n");
          //          printf("Going to IDLE state\n");
          stepper_pitch.step(-1 * current_pitch_position);
          current_pitch_position = 0;
          stepper_step.step(-1 * current_step_position);
          current_step_position = 0;
          looping = 0;
          mode = 0;
          exit_beep();
        }

        current_step = current_step + 1;
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
    enter_beep();
    //    printf("Mode: RESET\n");
    //    printf("Clearing Recording Memeory\n");
    for (int i = 0; i < recording_size; i++)
    {
      recording[i] = -1;
    }
    //    printf("Recording Memeory Cleared\n");
    //    printf("Going to IDLE State\n");
    stepper_pitch.step(-1 * current_pitch_position);
    current_pitch_position = 0;
    stepper_step.step(-1 * current_step_position);
    current_step_position = 0;
    mode = 0;
    exit_beep();
  }
}
