#include <SoftwareSerial.h>


int rxPin=2;
int txPin=3;

SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available()) {
    String readString="";
    while (mySerial.available()) {
      delay(2);
      char c=mySerial.read();
      readString += c;
    }
    Serial.print("message from exp:");
    Serial.println(readString);
  }
}
