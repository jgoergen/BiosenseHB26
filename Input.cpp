#include "Arduino.h"
#include "Input.h"
// you need this library to use code: https://github.com/JonHub/Filters
#include "Filters.h"  

// pin definitions
#define BTN_1_PIN     2
#define BTN_2_PIN     3
#define BTN_3_PIN     4
#define BTN_4_PIN     5

// only using one filter for all three kinds of data because I don't plan to pull more then one in a session.
FilterOnePole filterOneLowpass( LOWPASS, 5.0 );
int reading = 0;

Input::Input() {
}

void Input::init() {

    pinMode(BTN_1_PIN, INPUT_PULLUP);
    pinMode(BTN_2_PIN, INPUT_PULLUP);
    pinMode(BTN_3_PIN, INPUT_PULLUP);
    pinMode(BTN_4_PIN, INPUT_PULLUP);
}

bool Input::getButtonPress(int which) {

    switch(which) {

        case 1:
            return digitalRead(BTN_1_PIN) == LOW;
            break;

        case 2:
            return digitalRead(BTN_2_PIN) == LOW;
            break;

        case 3:
            return digitalRead(BTN_3_PIN) == LOW;
            break;

        case 4:
            return digitalRead(BTN_4_PIN) == LOW;
            break;
    }
}

int Input::getPulseData(bool filtered) { 

    reading = analogRead(A0);
    filterOneLowpass.input(reading); 

    if (filtered)
        return filterOneLowpass.output(); 
    else
        return reading;
}

int Input::getECGData(bool filtered) {

    reading = analogRead(A1);
    filterOneLowpass.input(reading); 

    if (filtered)
        return filterOneLowpass.output(); 
    else
        return reading; 
}

int Input::getEMGData(bool filtered) {

    reading = analogRead(A2);
    filterOneLowpass.input(reading); 

    if (filtered)
        return filterOneLowpass.output(); 
    else
        return reading;
}