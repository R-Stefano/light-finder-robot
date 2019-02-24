#include <dht.h>

dht DHT; //HUmidity sensor library

int LED_signal=2; //where to send signals to the LED

int DHV5=12;
int DH_signal=11;
int DHGND=10;

int BUZZ_signal=9;
int BUZZV5=8;
int BUZZGND=7;

int echoPin=6;
int trigPin=5;

struct DHSens {
  double temp;
  double humidity;
};

DHSens dhsensor;

void setup() {
  Serial.begin(9600);  //initial the Serial
  
  //Setup LED
  pinMode(LED_signal, OUTPUT); 
  
  //Setup DH
  pinMode(DHV5, OUTPUT);
  pinMode(DH_signal, INPUT);
  pinMode(DHGND, OUTPUT);

  //Keep the signals consistent to power DH
  digitalWrite(DHV5, HIGH);
  digitalWrite(DHGND, LOW);

  //Setup BUZZER
  pinMode(BUZZGND, OUTPUT);
  pinMode(BUZZ_signal, OUTPUT);
  pinMode(BUZZV5, OUTPUT);

  //Keep the signals consistent to power BUZZER
  digitalWrite(BUZZV5, HIGH);
  digitalWrite(BUZZGND, LOW);

  //SETUP ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {
  //digitalWrite(LED_signal, HIGH);
  getHumidityData();
  getDistance();
  blinkBuzzer(500,1000); // for 1 sec at 1Hz
  delay(2000);
  //digitalWrite(LED_signal, LOW);
  //blinkBuzzer(1000,2000); //for 1 sec at 2Hz
  //delay(2000);
}

void getHumidityData() {
  int chk = DHT.read11(DH_signal);
  //Store results in the object
  dhsensor.temp=DHT.temperature;
  dhsensor.humidity=DHT.humidity;
  Serial.print("Temperature = ");
  Serial.println(dhsensor.temp);
  Serial.print("Humidity = ");
  Serial.println(dhsensor.humidity);
}

void blinkBuzzer(int milliseconds, int frequency) {
  tone(BUZZ_signal, frequency);
  delay(milliseconds);
  noTone(BUZZ_signal);
}

void getDistance() {
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
  int distance=interval*0.034/2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
