int sensInputR=A7;
int sensInputL=A6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensValueR=analogRead(sensInputR);
  int sensValueL=analogRead(sensInputL);
  Serial.print("Right:");
  Serial.println(sensValueR);
  Serial.print("Left:");
  Serial.println(sensValueL);
  Serial.println("");
  delay(1000);
}
