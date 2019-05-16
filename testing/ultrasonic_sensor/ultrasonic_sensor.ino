//Ultrasonic sensor setup
int echoPin=6;
int trigPin=7;

void setup() {
  Serial.begin(9600);
  //SETUP ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int d=getDistance();
  Serial.print(d);
  Serial.print("mm");
  Serial.println("");
  delay(300);
}

//Return closest object in millimeters
int getDistance() {
  //Clear the pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //Set the trigPin on HIGH state for 10 micros
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Read the sound wave time in microseconds
  long interval=pulseIn(echoPin,HIGH);

  //calculate distance
  int d=interval*0.34/2;

  return d;
}
