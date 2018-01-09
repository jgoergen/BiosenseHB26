#include "Arduino.h"
 
#ifndef Input_h
#define Input_h
 
class Input
{
  public:
    Input(int button_pin_1, int button_pin_2, int button_pin_3, int button_pin_4);
    void init();
    bool getButtonPress(int which);
    int getBioSensorData(int which);
     
  private:
    int _button_pin_1;
    int _button_pin_2;
    int _button_pin_3;
    int _button_pin_4;
};
 
#endif
