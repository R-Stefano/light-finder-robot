//#include <SSD1306AsciiWire.h>
//#include <SSD1306AsciiSpi.h>
//#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>
//#include <SSD1306AsciiSoftSpi.h>
//#include <SSD1306init.h>
#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;

void setup() {                
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.clear();

  // first row
  oled.println("set1X test");

  // second row
  oled.set2X();
  oled.println("set2X test");

  // third row
  oled.set1X();
  oled.print("micros: ");
  oled.print(micros() - 10);
}
//------------------------------------------------------------------------------
void loop() {}
