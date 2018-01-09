#include "Arduino.h"
 
#ifndef Buzzer_h
#define Buzzer_h
 
class Buzzer
{
  public:    
    Buzzer();
    void mute();
    void unmute();
    void init();
    void beep(int bCount,int bDelay);
     
  private:
    int _pin;
    bool _muted;
};
 
#endif
