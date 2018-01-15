// includes
#include "Buzzer.h"
#include "OledDisplay.h"
#include "Input.h"

// you need this library to use code: https://github.com/JonHub/Filters

// component instantiation
Buzzer buzzer;
OledDisplay oledDisplay;
Input input;

// system variables
int appMode = 0;
String modeName;
int sensorMode = 0;

// averaging and graphing variables
float highestValueAverage = 0.0f;
float lowestValueAverage = 1025.0f;
float framesSinceLastLowest = 0;
float framesSinceLastHighest = 0;
float valueAverage = 0.0f;
float scale = 1; 
float mappedVal = 0.0f;
int x = 0; 
int graphHeight = 0;
int lastX = 0;
int lastY = 0;

// pulse variables
int LastTime = 0;
int ThisTime;
bool BPMTiming = false;
bool BeatComplete = false;
int BPM = 0;
int newBPM = 0;
float topLimit = 0.0f;
float bottomLimit = 0.0f;
int timesBPMWasClose = 0;

char outputBuffer[55]; 

void setup() {

    // initialize serial communications
    Serial.begin(115200);   

    // initialize buzzer system
    buzzer.init();  

    // mute sounds for now
    buzzer.mute(); 
    buzzer.beep(200,600);

    // initialize display
    oledDisplay.init();  

    // initialize input system
    input.init();   

    // start main menu display
    changeAppMode(0);
}

void loop() {

    switch(appMode) {

        case 0:
            mainMenuUpdate();
            break;

        case 1:
            EMGUpdate();
            break;

        case 2:
            ECGUpdate();
            break;

        case 3:
            pulseUpdate();
            break;
    }

    oledDisplay.display();
}

void changeAppMode(int mode) {

    appMode = mode;

    switch(mode) {

        case 0: 
            mainMenuStart();
            break;

        case 1:
            EMGStart();
            break;

        case 2:
            ECGStart();
            break;

        case 3:
            pulseStart();
            break;
    }
}

// ################## MAIN MENU ##################

void mainMenuStart() {

    oledDisplay.clear();
    oledDisplay.print(10, 1, "3. EMG");   // align menu to labels on buttons
    oledDisplay.print(10, 20, "4. ECG/EEG");
    oledDisplay.print(10, 40, "5. PULSE");

    buzzer.beep(200,600);
    buzzer.beep(300,200);
    buzzer.beep(400,300);
}

void mainMenuUpdate() {

    if (input.getButtonPress(1))
        changeAppMode(1);
    else if (input.getButtonPress(2))
        changeAppMode(2);
    else if (input.getButtonPress(3))
        changeAppMode(3);
    
    // there should be no need to update more then 30fps here
    delay(33);
}

void computeAverageValues(int value) {

    valueAverage = (valueAverage + value) / 2.0f;  

    if (valueAverage > highestValueAverage) {

        highestValueAverage = (highestValueAverage + valueAverage) / 2.0f; 
        framesSinceLastHighest = 0;

    } else {

        framesSinceLastHighest ++;

        //if it's been more then 20 frames since the last expansion, then contract some to keep the bounds correct.
        if (framesSinceLastHighest > 20)
            highestValueAverage -= 0.3f;
    }

    if (valueAverage < lowestValueAverage) {
        
        lowestValueAverage = (lowestValueAverage + valueAverage) / 2.0f; 
        framesSinceLastLowest = 0;
    } else {

        framesSinceLastLowest ++;

        //if it's been more then 20 frames since the last expansion, then contract some to keep the bounds correct.
        if (framesSinceLastLowest > 20)
            lowestValueAverage += 0.2f;
    }
}

void resetAverages(int value) {

    valueAverage = value;
    highestValueAverage = 0.0f;
    lowestValueAverage = 1025.0f;
}

void displayData(int value, int topPadding, int bottomPadding) {

    // reset bounds and redraw text info
    if (x > 128) {

        // only erase the graphing area so we preserve any items added to the display by other code ( title, bpm, etc. this is why you pass padding to this function. )
        oledDisplay.eraseRect(0, topPadding, 128, 64 - topPadding - bottomPadding);
        x = 0;
        lastX = 0;
    }

    graphHeight = 64 - topPadding - bottomPadding;
    mappedVal = (int)map(valueAverage, lowestValueAverage, highestValueAverage, 0, graphHeight);

    oledDisplay.drawLine(
        lastX, 
        lastY, 
        x, 
        topPadding + (mappedVal - (graphHeight / 2)) + (graphHeight / 2) - 1);

    lastX = x;
    lastY = topPadding + (mappedVal - (graphHeight / 2)) + (graphHeight / 2) - 1;
    x ++;  
}