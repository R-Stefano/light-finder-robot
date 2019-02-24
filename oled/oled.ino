//OLED SCREEN LIBRARIES
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Create OLED object for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  
  //Setup the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the display
  display.clearDisplay();
  display.display();

  //Set text style
  display.setTextColor(WHITE);
  display.setTextSize(1); //from 1 to 8
  //font-size=1: height 8px
  //font-size=2: height 15px
  
  // Draw a single pixel in white
  //display.drawPixel(10, 10, WHITE);
}


void loop() {
  String msg=readInput("Insert messsage (INSERT 'reset' for cleaning the screen):");
  if (msg=="reset") {
    clearDisplay();
  } else {
    String x=readInput("X coordinates");
    String y=readInput("Y coordinates");
    OLEDMessage(msg, x.toInt(), y.toInt());
  }

}

void clearDisplay() {
  display.clearDisplay();
  display.display();
}

void OLEDMessage(String message, int x, int y) {
  //set coordinates where start writing
  display.setCursor(x,y);
  
  //Write message
  display.print(message);
  
  //Display the message
  display.display();
}

String readInput(String msg) {
  Serial.println(msg);
  while(!Serial.available()) {
    // wait for input
  }

  return Serial.readStringUntil(10);
}
