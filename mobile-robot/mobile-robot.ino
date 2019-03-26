int echoPin=12;
int trigPin=11;

//LEDs: GREEN, YELLOW, RED
int LEDS[]= {8,9,10};

//LDR
int LDR=A0;

//It stores the cm distance from a wall
int distanceWall;

//it sotres the threshold of the distances
int distancesThre[]= {10,30};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //initial the Serial

  //SETUP LEDs
  for (int i = 0; i < sizeof(LEDS); i++) {
    pinMode(LEDS[i], OUTPUT);
    digitalWrite(LEDS[i], LOW);
  }
  
  //SETUP ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {
  distanceWall=getDistance();
  if (distanceWall <= distancesThre[0]) {
    Serial.print("Red zone, Distance: ");
    Serial.print(distanceWall);
    Serial.println(" cm");
    toggleLED(2);
    //stop
  } else if (distanceWall <= distancesThre[1]) {
    Serial.print("Yellow zone, Distance: ");
    Serial.print(distanceWall);
    Serial.println(" cm");
    toggleLED(1);
    //Slowing down
  } else {
    Serial.print("Green zone, Distance: ");
    Serial.print(distanceWall);
    Serial.println(" cm");
    toggleLED(0);
    //Moving forward
  }

  //LDR setup
  int sensorValue=analogRead(LDR);
  Serial.print("LDR measurement 0-1024: ");
  Serial.println(sensorValue);
  delay(200);
}

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
  int d=interval*0.034/2;

  return d;
}

void toggleLED(int ledON) {
  //Turn off all the LEDS
  for (int i = 0; i < sizeof(LEDS); i++) {
    digitalWrite(LEDS[i], LOW);
  }

  //Turn on the defined LED
  digitalWrite(LEDS[ledON], HIGH);
}

