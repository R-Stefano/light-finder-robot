//OLED SCREEN LIBRARIES
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//OLED setup
//Create OLED object for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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

  display.clearDisplay();
}

void loop() {
  delay(50000);
  Serial.println("updating screen");
  //Reset the screen
  display.clearDisplay();
  //display.display();

  //Display ultrasonic values
  String msg="Hello world";
  OLEDMessage(msg, 5,15);
  OLEDMessage(msg, 5,25);
  delay(2000);
}

void OLEDMessage(String message, int x, int y) {
  //set coordinates where start writing
  display.setCursor(x,y);
  
  //Write message
  display.print(message);
  
  //Display the message
  display.display();
}
