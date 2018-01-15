#include "Arduino.h"

#ifndef Buzzer_h
#define Buzzer_h

class Buzzer
{
  public:    
    Buzzer(int _pin);
    void mute();
    void unmute();
    void init();
    void beep(int bCount,int bDelay);
      
  private:
    bool _muted;
    int _pin;
};

#endif