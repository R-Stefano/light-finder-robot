
//temp sensor library
#include <dht.h>
//OLED SCREEN LIBRARIES
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//OLED setup
//Create OLED object for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Humidity sensor setup
dht DHT; //HUmidity sensor library

struct DHSens {
  int temp;
  int humidity;
};

DHSens dhsensor;

int DH_signal=7;

//Ultrasonic sensor setup
int echoPin=12;
int trigPin=11;

  //It stores the cm distance from a wall
int distanceWall;

  //it sotres the threshold of the distances
int distancesThre[]= {150,300};
int distGap=30;

int list_size=10;
int measures[]={0,0,0,0,0,0,0,0,0,0};

//LEDs: RED, GREEN, BLUE
int LEDS[]= {A1,A2,A3};

//LDR
int LDR=A0;

//set how long between distance measurements
int loop_delay=10;
//manage every how much times update the temperature screen
int loop_counter=0;
int nIterations=100; //100*nIterations(10)= 2s
String zone;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //initial the Serial

  //Setup the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  } else {
    Serial.println("OLED successfully started!");
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the display
  display.clearDisplay();
  display.display();

  //Set display text style
  display.setTextColor(WHITE);
  display.setTextSize(1); //from 1 to 8
  
  //Setup humidity/tempo sensor
  pinMode(DH_signal, INPUT);

  //SETUP LEDs
  for (int i = 0; i < sizeof(LEDS); i++) {
    pinMode(LEDS[i], OUTPUT);
    analogWrite(LEDS[i], 0);
  }
  
  //SETUP ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {
  distanceWall=updateList(measures, getDistance());
  //Turn off all the LEDS
  for (int i = 0; i < sizeof(LEDS); i++) {
    analogWrite(LEDS[i], 0);
  }
  if (distanceWall <= distancesThre[0]) {
    float scale=float(distanceWall)/float(distancesThre[0]);
    int value=(int)(scale*255);
    Serial.println("first condition" + String(scale));
    setLEDColor(255, value, 0);
  } else {    
    float scale=1 - (float(distanceWall)/float(distancesThre[1]));
    scale= scale>=0? scale : 0;
    Serial.println("second condition" + String(scale));
    int value=(int)(scale*255);
    setLEDColor(value, 255, 0);    
  }
  if (distanceWall <= (distancesThre[0]-distGap)) {
    zone="Red";
  } else if (distanceWall <= (distancesThre[0]+distGap)){
    zone="Red/Yellow";
  } else if (distanceWall <= (distancesThre[1]-distGap)) {
    zone="Yellow";
  } else if (distanceWall <= (distancesThre[1]+ distGap)) {
    zone="Yellow/Green";
  } else {
    zone="Green";
  }

  //LDR setup
  int sensorValue=analogRead(LDR);
  //Serial.print("LDR measurement 0-1024: ");
  //Serial.println(sensorValue);

  if(loop_counter%nIterations==0) {
    Serial.println("updating screen");
    //Reset the screen
    display.clearDisplay();
    //display.display();
    //Retrieve humidity and temperature and set on OLED
    getHumidityData();
    //Display ultrasonic values
    String msg="Obstacle (mm): " + String(distanceWall);
    OLEDMessage(msg, 5,25);
    msg="Zone: " + zone;
    OLEDMessage(msg, 5,35);
  }

  delay(loop_delay);
  loop_counter+=1;
}
void setLEDColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(A1, red);
  analogWrite(A2, green);
  analogWrite(A3, blue);  
}

int updateList(int mylist[], int value) {
  int counter=value;
  for (int i=list_size-1; i>0; i--) {
    mylist[i]=mylist[i-1];
    counter +=mylist[i-1];
  }
  mylist[0]=value;
  int avg_value=(int) (counter/list_size);
  return avg_value;
  
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
  int d=interval*0.34/2;

  return d;
}

void getHumidityData() {
  int chk = DHT.read11(DH_signal);
  //Store results in the object
  dhsensor.temp=(int)DHT.temperature;
  dhsensor.humidity=(int)DHT.humidity;
  OLEDMessage(("Temperature: "+ (String)dhsensor.temp), 5,5);
  OLEDMessage(("Humidity: " + (String)dhsensor.humidity), 5,15);
}

void OLEDMessage(String message, int x, int y) {
  //set coordinates where start writing
  display.setCursor(x,y);
  
  //Write message
  display.print(message);
  
  //Display the message
  display.display();
}
