#include "Arduino.h"
#include "Buzzer.h"

#define BUZZER_PIN 12

static bool _muted = false;

Buzzer::Buzzer() {
}

void Buzzer::init() {

    pinMode(BUZZER_PIN, OUTPUT);
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

        digitalWrite(BUZZER_PIN, i&1);

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

    digitalWrite(BUZZER_PIN, LOW);
}