/*
Adafruit Arduino - Lesson 10. Simple Sounds
*/
 
int speakerPin = 12;
 
int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//            mid C  C#   D    D#   E    F    F#   G    G#   A
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Tone test!");

}
 
void loop()
{
    for (int i = 0; i < numTones; i++)
  {
    Serial.println("Tone...");
    tone(speakerPin, tones[i]);
    delay(500);
  }
  noTone(speakerPin);
}
