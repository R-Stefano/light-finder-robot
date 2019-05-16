//motorA (right wheel)
int forwA=2;
int backA=3;

//motorB
int forwB=4;
int backB=5;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(backA, OUTPUT);
  pinMode(forwA, OUTPUT);
  pinMode(forwB, OUTPUT);
  pinMode(backB, OUTPUT);
  Serial.println("f: forward");
  Serial.println("b: backward");
  Serial.println("r: right");
  Serial.println("l: left");
  Serial.println("s: stop");
}

void loop() {
  if(Serial.available() > 0) {
    int inByte=Serial.read();
    updateDirection(inByte);    
  }
}

void updateDirection(int u_input) {
  //u_input is the char number, the switch statement
  //automatically convert it to its repsective character
    switch(u_input) {
      case 'f':
        Serial.println("Moving: forward");
        sendToWheels(1, 1, 0, 0);
        break;
      case 'b':
        Serial.println("Moving: back");
        sendToWheels(0, 0, 1, 1);
        break;
      case 'l':
        Serial.println("Moving: left");
        sendToWheels(1, 0, 0, 1);
        break;
      case 'r':
        Serial.println("Moving: right");
        sendToWheels(0, 1, 1, 0);
        break;
      case 's':
        Serial.println("Stop");
        sendToWheels(0,0,0,0);
        break;
    }
}

void sendToWheels(int v1,int v2,int v3,int v4) {
  digitalWrite(forwA, v1);
  digitalWrite(forwB, v2);
  digitalWrite(backA, v3);
  digitalWrite(backB, v4);
}
