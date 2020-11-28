int mode = 1;

int step_time = 0.5;
int step_gate = 0.5;
int loop_length = 4;
int current_step = 0;
int looping = 0;
int current_step_pitch = 0;

int recording[16];
int scale_a_minor[12] = {440,466,494,523,554,587,622,659,698,740,784,840};

int analog_note_inputs[4] = {A0,A1,A2,A3};
int audio_signal_output = 12;

volatile int wait=1000;

void setup() {
  Serial.begin(115200);
}


void loop() {
  // IDLE
  if(mode == 0){
    Serial.println("Mode: IDLE");
    
  }
  // 
  if(mode == 1){
    Serial.println("Mode: LIVE SEQUENCE");
    looping = 1;
    current_step = 0;
    
    while(looping){
      Serial.print("Step: ");
      Serial.print(current_step);
      Serial.print("\n");
      
//      current_step_pitch = map(analogRead(analog_note_inputs[current_step]), 0, 1023, 0, 11);
      current_step_pitch = map(analogRead(analog_note_inputs[current_step]), 0, 1023, 0, 11);
      Serial.print("Pitch: ");
      Serial.print(current_step_pitch);
      Serial.print("\n");

      tone(audio_signal_output, scale_a_minor[current_step_pitch], 100);
      wait = int(1000.0 * step_time);
      Serial.print(wait);
      delay(wait);
      Serial.print("\n");
      
      
      current_step++;
      if(current_step>=loop_length){
        current_step=0;
      }
    }
  }
  else{
  mode = 1;
  }
  
}
