#include "Arduino.h"
#include "Buzzer.h"
 
static int _pin;
static bool _muted = false;
 
Buzzer::Buzzer(int pin) {
   
  _pin = pin;
}
 
void Buzzer::init() {
   
  pinMode(_pin, OUTPUT);
}

void Buzzer::mute() {
   
  _muted = true;
}

void Buzzer::unmute() {
   
  _muted = false;
}
 
void Buzzer::beep(int bCount,int bDelay) {

  if (_muted)
    return;
  
  for (int i = 0; i <= bCount * 2; i++) {
    
    digitalWrite(_pin, i&1);
    
    for(int i2 = 0; i2 < bDelay; i2++) {
        
      __asm__("nop\n\t");
      
      #if F_CPU > 8000000
        __asm__("nop\n\t");
        __asm__("nop\n\t");
        __asm__("nop\n\t");
        __asm__("nop\n\t");
        __asm__("nop\n\t");
        __asm__("nop\n\t");
        __asm__("nop\n\t");
        __asm__("nop\n\t");
      #endif
    }
  }
  
  digitalWrite(_pin, LOW);
}
