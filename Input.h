#include "Arduino.h"
 
#ifndef Input_h
#define Input_h
 
class Input
{
  public:
    Input();
    void init();
    bool getButtonPress(int which);
    int getPulseData();
    int getECGData();
    int getEMGData();
     
  private:
    int _button_pin_1;
    int _button_pin_2;
    int _button_pin_3;
    int _button_pin_4;
};
 
#endif
