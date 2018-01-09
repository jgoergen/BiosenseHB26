#include "Arduino.h"
#include "Input.h"

static int _button_pin_1;
static int _button_pin_2;
static int _button_pin_3;
static int _button_pin_4;
 
Input::Input(int button_pin_1, int button_pin_2, int button_pin_3, int button_pin_4) {
   
  _button_pin_1 = button_pin_1;
  _button_pin_2 = button_pin_2;
  _button_pin_3 = button_pin_3;
  _button_pin_4 = button_pin_4;
}
 
void Input::init() {
   
  pinMode(_button_pin_1, INPUT_PULLUP);
  pinMode(_button_pin_2, INPUT_PULLUP);
  pinMode(_button_pin_3, INPUT_PULLUP);
  pinMode(_button_pin_4, INPUT_PULLUP);
}

bool Input::getButtonPress(int which) {

  switch(which) {
    
    case 1:
      return digitalRead(_button_pin_1) == LOW;
      break;
      
    case 2:
      return digitalRead(_button_pin_2) == LOW;
      break;
      
    case 3:
      return digitalRead(_button_pin_3) == LOW;
      break;
      
    case 4:
      return digitalRead(_button_pin_4) == LOW;
      break;
  }
}
 
int Input::getBioSensorData(int which) {

  return analogRead(which); // emg - A2, ecg/eeg - A1, pulse - A0
}
