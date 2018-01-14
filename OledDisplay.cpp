// https://github.com/adafruit/Adafruit_SSD1306

#include "Arduino.h"
#include "OledDisplay.h"
#include "Adafruit_SSD1306.h"

#define OLED_Address 0x3C
Adafruit_SSD1306 oled(1);
 
OledDisplay::OledDisplay() {
   
}
 
void OledDisplay::init() {
   
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.clearDisplay();
  oled.display();
}

void OledDisplay::clear() {

  oled.clearDisplay();
}

void OledDisplay::print(int x, int y, char message[]) {

  oled.setCursor(x, y);
  oled.setTextSize(1);
  oled.println(message);
}
void OledDisplay::print(int x, int y, String message) {

  oled.setCursor(x, y);
  oled.setTextSize(1);
  oled.println(message);
}

void OledDisplay::printLarge(int x, int y, char message[]) {
  oled.setCursor(x, y);
  oled.setTextSize(2);
  oled.println(message);
}

void OledDisplay::printLarge(int x, int y, String message) {
  oled.setCursor(x, y);
  oled.setTextSize(2);
  oled.println(message);
}


void OledDisplay::drawLine(int x1, int y1, int x2, int y2) {

  oled.writeLine(x1, y1, x2, y2, WHITE);
}

void OledDisplay::eraseLine(int x1, int y1, int x2, int y2) {

  oled.writeLine(x1, y1, x2, y2, BLACK);
}

void OledDisplay::fillRect(int x1, int y1, int x2, int y2) {

  oled.writeFillRect(x1, y1, x2, y2, WHITE);
}

void OledDisplay::eraseRect(int x1, int y1, int x2, int y2) {

  oled.writeFillRect(x1, y1, x2, y2, BLACK);
}

void OledDisplay::drawPixel(int x, int y) {

  oled.drawPixel(x, y, WHITE);
}

void OledDisplay::erasePixel(int x, int y) {

  oled.drawPixel(x, y, BLACK);
}
 
void OledDisplay::display() {

  oled.display();
}
 
