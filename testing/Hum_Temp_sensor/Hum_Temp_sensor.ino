//temp sensor library
#include <dht.h>

//Humidity sensor setup
dht DHT;

struct DHSens {
  int temp;
  int humidity;
};

DHSens dhsensor;

int DH_signal=10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Setup humidity/tempo sensor
  pinMode(DH_signal, INPUT);
}

void loop() {
  Serial.println("Looping");
  // put your main code here, to run repeatedly:
  //Retrieve humidity and temperature and set on OLED
  getHumidityData();
  delay(2000);
}

void getHumidityData() {
  int chk = DHT.read11(DH_signal);
  //Store results in the object
  dhsensor.temp=(int)DHT.temperature;
  dhsensor.humidity=(int)DHT.humidity;
  Serial.println(("Temperature: "+ (String)dhsensor.temp));
  Serial.println(("Humidity: " + (String)dhsensor.humidity));
  //OLEDMessage(("Temperature: "+ (String)dhsensor.temp), 5,5);
  //OLEDMessage(("Humidity: " + (String)dhsensor.humidity), 5,15);
}
