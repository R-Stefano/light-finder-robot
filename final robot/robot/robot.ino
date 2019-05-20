//temp sensor library
#include <dht.h>
//LITE OLED SCREEN LIBRARIES
//#include <SSD1306AsciiWire.h>
//#include <SSD1306AsciiSpi.h>
//#include <SSD1306Ascii.h>
//#include <SSD1306AsciiSoftSpi.h>
//#include <SSD1306init.h>
#include <SSD1306AsciiAvrI2c.h>
#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;

//Humidity sensor setup
dht DHT;
struct DHSens {
  int temp;
  int humidity;
};
DHSens dhsensor;

//motors, (A right wheel)
int forwA=3;
int backA=2;
int forwB=5;
int backB=4;
//LDRs
int LDRRight=A6;
int LDRLeft=A7;
//Ultrasonic sensor setup
int echoPin=6;
int trigPin=7;
//LEDs setup
int rLED=9;
int lLED=8;
//RGBLEDs: RED, GREEN, BLUE
int redPin=A0;
int greenPin=A1;
int bluePin=A2;
int RGBLEDs[]= {redPin,bluePin,greenPin};
//Humidity, temperature sensor
int DH_signal=11;
//Buzzer
int buzzer = 10;
//pin for the boot button
int btnPin=12;

//VARIABLES
//store the values for LDR sensors
int const buffersSize=5; //past history of light values
int RLightsBuffer[buffersSize]; //buffer to remove variance of light measurement
int LLightsBuffer[buffersSize]; //buffer to remove variance of light measurement
int RLLights[2]; //averaged values
int lightsDiffThreshold=5; //How mich the two LDRs values should differ for changing direction
int environmentLightValue; //an average of a 360deg measurement of the environment

//store the values for ultrasonic sensor
int distancesThre[]= {150,600};//First value: threshold min dist(R), Second value: threshold (Y-G). In mm
int bufferDistances[] = {150,150,150}; //buffer to remove variance
int backDirectionTime=1000;//ms, time to go backward when obstacle encountered
int measureDelays=100;//ms, time between measurements when obstacle encountered
int distBufferSize=3; //past history of distances
int avgDistance; //averaged value of distance
int distGap=30;

int rotationTime=750; //for 45 deg
int timeCounter=0; //keep track of how many 0.2s iterations. Used for every 2 sec the hum sensor
boolean isBooted=false; //when set to true, start everything
void setup() {
  Serial.begin(9600);
  //SETUP DC Motor
  pinMode(backA, OUTPUT);
  pinMode(forwA, OUTPUT);
  pinMode(forwB, OUTPUT);
  pinMode(backB, OUTPUT);

  //SETUP ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  //SETUP LEDs
  pinMode(rLED, OUTPUT);
  pinMode(lLED, OUTPUT);
  digitalWrite(rLED, LOW);
  digitalWrite(lLED, LOW);
  
  //SETUP RGBLEDs
  for (int i = 0; i < 3; i++) {
    pinMode(RGBLEDs[i], OUTPUT);
    analogWrite(RGBLEDs[i], 0);
  }

  //Setup humidity/tempo sensor
  pinMode(DH_signal, INPUT);

  //Setup buzzer
  pinMode(buzzer, OUTPUT);

  //setup boot button
  pinMode(btnPin, INPUT);
  digitalWrite(btnPin, LOW);

  //Setup the display
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.clear();
}

void loop() {
  if (isBooted) {
    mainLoop();
  } else if (digitalRead(btnPin)) {
    configuration();
  }

}

void mainLoop() {
  //update light value measurement
  updateLightsValues();
  //update distance measurement
  updateDistanceValue();
  //update RGBLED color
  updateObstacleDistanceStatus(avgDistance);
  //check if the closest object is above the minimum distance threshold
  if (avgDistance< distancesThre[0] && ((RLLights[0] + RLLights[1])/2)>environmentLightValue*2) {
    sendData("Light source found!", "console");
    updateMainRoutineOnDisplay("Light source found!");
    updateDirection((char)'s');
    sendData("Celebrating!", "console");
    updateMainRoutineOnDisplay("Celebrating!");
    updateDirection((char)'l');
    activateBuzzer(500,250,10);
    updateDirection((char)'r');
    activateBuzzer(2000,250,10);
    updateDirection((char)'s');

    //shut down
    isBooted=false;
    oled.clear();//clear display
    //turnoff leds
    for (int i = 0; i < 3; i++) {
      analogWrite(RGBLEDs[i], 0);
    }
    digitalWrite(rLED, LOW);
    digitalWrite(lLED, LOW);
    sendData("Shut down..", "console");
    
  } else if (avgDistance< distancesThre[0]) {
    sendData("Handling obstacle..", "console");
    updateMainRoutineOnDisplay("Handling obstacle..");
    //stop the robot
    updateDirection((char)'s');
    //sound of obstacle found
    activateBuzzer(1000,1000,1);
    //turn off lights direction LEDS
    manageDirectionLEDs(0,0);
    //move back to increase manouvre space
    updateDirection((char)'b');
    delay(backDirectionTime);
    updateDirection((char)'s');
    //Check for new directions
    findNewRoute();
  } else {
    //sendData("Following light..", "console");
    updateMainRoutineOnDisplay("Following light..");
    //keep adjusting the direction towards the light source
    assessDirection();
  }

  //every second update the screen
  if (timeCounter%1000==0) {
    //sendData("Updating screen..", "console");
    DisplayInformation();

    timeCounter=0;
  }
  
  timeCounter += 10;
  delay(10);
}

void configuration() {
    sendData("Configuring robot..", "console");
    sendData("1. Testing motors..", "console");
    oled.println("Configuring robot..");
    oled.println("1. Testing motors..");
    //testing motors, forw, backw, make 360 degree wheels turn, left and right
    updateDirection((char)'f');
    delay(500);
    updateDirection((char)'b');
    delay(500);
    updateDirection((char)'l');
    delay(rotationTime*8);
    updateDirection((char)'r');
    delay(rotationTime*8);
    updateDirection((char)'s');

    sendData("2. Scanning environment for light sources..", "console");
    oled.println("2. Scanning environment for");
    oled.println("light sources..");
    //360degree scanning for light source
    scanEnvLights();

    isBooted=true;
    delay(1000);
    DisplayInformation();
}

//This function is used to asses which direction
//(left or right or neither) the robot should move based on the light source
void assessDirection() {
  long diff=RLLights[0] - RLLights[1];
  int threshold=lightsDiffThreshold; //move right, left only if above this number

  if ( diff > threshold) {
    //move right
    updateDirection((char)'r');
    manageDirectionLEDs(1,0);
  } else if (diff < -threshold) {
    //move left
    updateDirection((char)'l');
    manageDirectionLEDs(0,1);
  } else {
    updateDirection((char)'f');
    manageDirectionLEDs(1,1);
  }
}

//if there is an obstacle, handle it
void findNewRoute(){
  //distances at -90, -45, 45, 90
  int distances[4];
  //control 45/90 deg on the right
  for (int i=0; i<2; i++) {
    //move right
    updateDirection((char)'r');
    delay(rotationTime); //45 deg
    updateDirection((char)'s');
    int totDist=0;
    for (int j=0; j<distBufferSize; j++) {
      totDist += getDistance();
      delay(measureDelays);
    }
    distances[i+2]=totDist/distBufferSize;
  }

  //come back to original direction
  updateDirection((char)'l');
  delay(rotationTime*2); 
  updateDirection((char)'s');

  //control 45/90 deg on the left
  for (int i=0; i<2; i++) {
    //move right
    updateDirection((char)'l');
    delay(rotationTime); //45 deg
    updateDirection((char)'s');
    int totDist=0;
    for (int j=0; j<distBufferSize; j++) {
      totDist += getDistance();
      delay(measureDelays);
    }
    distances[i]=totDist/distBufferSize;
  }
  //get best route
  int idx=0;
  int value=0;
  for (int i=0; i<4; i++) {
    if (distances[i]>value) {
      idx=i;
      value=distances[i];
    }
  }

  //Set robot in optimal direction
  updateDirection((char)'r');
  //starting at -45, how many on the right
  delay(rotationTime*idx); 
  //move forward to overcome obstacle
  updateDirection((char)'f');
  delay(1000);
}

void updateDistanceValue() {
  int d=getDistance();

  long tot=0;
  for (int i=0; i<distBufferSize-1; i++) {
    bufferDistances[i]=bufferDistances[i+1];
    tot += bufferDistances[i];
  }
  bufferDistances[distBufferSize-1]=d;
  
  avgDistance=(d + tot)/distBufferSize;
}

//This function is used to update the values for the lights measurements
void updateLightsValues() {
  //get new reading
  int newValueRight=analogRead(LDRRight);
  int newValueLeft=analogRead(LDRLeft);

  //compute the average
  long sumRight=0;
  long sumLeft=0;
  for (int i=0; i< buffersSize-1; i++) {
    //shift old values by 1
    RLightsBuffer[i]=RLightsBuffer[i+1];
    LLightsBuffer[i]=LLightsBuffer[i+1];
    
    //keep track of the sum
    sumRight += RLightsBuffer[i];
    sumLeft += LLightsBuffer[i];
  }
  
  //insert the last values in the buffer
  RLightsBuffer[buffersSize-1]= newValueRight;
  LLightsBuffer[buffersSize-1]= newValueLeft;

  //update average
  RLLights[0]=(sumRight + newValueRight)/buffersSize;
  RLLights[1]=(sumLeft + newValueLeft)/buffersSize;
}

//360degree scanning, move by 45 deg each time
void scanEnvLights() {
  int MaxValue=0;
  int direcIdxMaxValue=0;
  long totalVals=0;
  for (int i=0; i<8; i++) {
    //Serial.print("Degree:");
    //Serial.println(i*45);
    //Make the light measurement
    int directionAvgValue=(analogRead(LDRRight) + analogRead(LDRLeft))/2;
    //check if new best value
    if (directionAvgValue>MaxValue){
      direcIdxMaxValue=i;
      MaxValue=directionAvgValue;
    }
    //make sound
    activateBuzzer(1000,500,1);
    //Move the robot
    updateDirection((char)'r');
    //let it turn by 45 degree
    delay(rotationTime);
    //Stop, let robot take measurement at next iteration
    updateDirection((char)'s');
    delay(rotationTime);

    totalVals += directionAvgValue;
  }

  //Set environemnt avg value
  environmentLightValue=totalVals/8;
  //Light source between 180-315 deg, move left
  int degIdx=direcIdxMaxValue-3;
  int w;
  if (degIdx>0) {
    updateDirection((char)'l');
    w=5-degIdx;
  } else {
  //Light source between 0-135 deg, move right
    updateDirection((char)'r');
    w=3-abs(degIdx);
  }
  delay(w*rotationTime);
  //move forward to overcome obstacle
  updateDirection((char)'f');
  delay(rotationTime*2);
}

//Convert the command into the actual input for the motors
void updateDirection(int u_input) {
  //u_input is the char number, the switch statement
  //automatically convert it to its repsective character
    switch(u_input) {
      case 'f':
        //Serial.println("Moving: forward");
        moveWheels(0, 0, 1, 1);
        break;
      case 'b':
        //Serial.println("Moving: back");
        moveWheels(1, 1, 0, 0);
        break;
      case 'l':
        //Serial.println("Moving: left");
        moveWheels(1, 0, 0, 1);
        break;
      case 'r':
        //Serial.println("Moving: right");
        moveWheels(0, 1, 1, 0);
        break;
      case 's':
        //Serial.println("Stop");
        moveWheels(0,0,0,0);
        break;
    }
}

//Once the direction has been defined, set it
void moveWheels(int v1,int v2,int v3,int v4) {
  digitalWrite(backA, v1);
  digitalWrite(backB, v2);
  digitalWrite(forwA, v3);
  digitalWrite(forwB, v4);
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

//Used to turn ON/OFF the directional LEDs (Right/Left)
void manageDirectionLEDs(int r, int l) {
  digitalWrite(rLED, r);
  digitalWrite(lLED, l);
}

//Called to udpate the color of RGBLEDs based on distance to obstacle
void updateObstacleDistanceStatus(int distance) {
  int midPoint=float(distancesThre[0])+ float(distancesThre[1])/float(2);

  float x;
  float y;
  if (avgDistance > midPoint) {
    x = 1 - (float(avgDistance)/float(distancesThre[1]));
    x = x<0 ? 0:  x;
    y = 1;
  } else {
    x = 1;
    y = float(avgDistance)/float(midPoint);
  }
  int valueX=(int)(x*255);
  int valueY=(int)(y*255);
  setRGBLEDColor(valueX, valueY, 0);
}

//Used to update the color of the RGBLED
void setRGBLEDColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

//Used to retrieved the new humidity data
void getHumidityData() {
  int chk = DHT.read11(DH_signal);
  //Store results in the object
  dhsensor.temp=(int)DHT.temperature;
  dhsensor.humidity=(int)DHT.humidity;
}

void activateBuzzer(int freq, int duration, int times) {
  for (int i=0; i<times; i++) {
    //Send freqKHz sound signal
    tone(buzzer, freq);
    //duration of the sound
    delay(duration);
    //Stop sound
    noTone(buzzer);
    delay(duration);
  }

}

void DisplayInformation() {
    //Reset the screen
     oled.clear();
    //It gets the new data and display
    getHumidityData();
    oled.println("Temperature: "+ (String)dhsensor.temp);
    oled.println("Humidity: " + (String)dhsensor.humidity);
    
    //display luminosity
    String msg="Light(R/L): " + String(RLLights[0]);
    msg +="/"+String(RLLights[1]);
    oled.println(msg);
    
    oled.print("Avg val:");
    oled.println(String(environmentLightValue));
    
    //Display distance to the wall
    String zone;
    if (avgDistance <= (distancesThre[0]-distGap)) {
      zone="Red";
    } else if (avgDistance <= (distancesThre[0]+distGap)){
      zone="Red/Yellow";
    } else if (avgDistance <= (distancesThre[1]-distGap)) {
      zone="Yellow";
    } else if (avgDistance <= (distancesThre[1]+ distGap)) {
      zone="Yellow/Green";
    } else {
      zone="Green";
    }

    msg="Obstacle at " + String(avgDistance);
    msg += "(mm)";
    oled.println(msg);
    oled.println("Zone: " + zone);

    String inputData="Temperature: "+ String(dhsensor.temp) + 
    "\nHumidity: " + String(dhsensor.humidity) +
    "\nLight(R/L): " + String(RLLights[0]) + "/" + String(RLLights[1]) +
    "\nEnvironment value: " + String(environmentLightValue) +
    "\nObstacle at " + String(avgDistance) + " mm" +
    "\nZone: " + String(zone);    

    sendData(inputData, "display");
}

void updateMainRoutineOnDisplay(String msg) {
    //display it on the 6th line, available {0-7}
    oled.setCursor(0, 6);
    oled.println(msg);
}

void sendData(String data, String type) {
  String inputData="$";
  inputData+= type + "=" + data;
  Serial.println(inputData);
}
