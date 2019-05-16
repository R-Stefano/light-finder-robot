int buzzer = 11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  activateBuzzer(1000,1000);
  delay(2000);
}

void activateBuzzer(int freq, int duration) {
  //Send freqKHz sound signal
  tone(buzzer, freq);
  //duration of the sound
  delay(duration);
  //Stop sound
  noTone(buzzer);
}
