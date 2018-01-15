#include "Arduino.h"
 
#ifndef OledDisplay_h
#define OledDisplay_h
 
class OledDisplay
{
  public:
    int Width;
    int Height;
    
    OledDisplay();
    void init();
    void clear();
    void print(int x, int y, char message[]);
    void print(int x, int y, String message);// accept strings
    void printLarge(int x, int y, char message[]);
    void printLarge(int x, int y, String message); // accept strings
    void drawPixel(int x, int y);
    void erasePixel(int x, int y);
    void drawLine(int x1, int y1, int x2, int y2);
    void eraseLine(int x1, int y1, int x2, int y2);
    void fillRect(int x1, int y1, int x2, int y2);
    void eraseRect(int x1, int y1, int x2, int y2);
    void display();
     
  private:
    
};
 
#endif
