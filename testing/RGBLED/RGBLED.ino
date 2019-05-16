//RGBLEDs: RED, GREEN, BLUE
int redPin=A0;
int bluePin=A1;
int greenPin=A2;
int RGBLEDs[]= {redPin,bluePin,greenPin};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //SETUP RGBLEDs
  for (int i = 0; i < 3; i++) {
    pinMode(RGBLEDs[i], OUTPUT);
    analogWrite(RGBLEDs[i], 0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  for (int r=0; r<25; r++) {
    for (int g=0; g<25; g++) {
      for (int b=0; b<25; b++) {
        setRGBLEDColor(r*10,g*10,b*10);
      }
    }
  }*/
  setRGBLEDColor(255, 255, 0);
}

void setRGBLEDColor(int red, int green, int blue)
{
  analogWrite(greenPin, green);
  analogWrite(redPin, red);
  analogWrite(bluePin, blue);  
}
