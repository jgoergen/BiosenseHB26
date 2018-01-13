// includes
#include "Buzzer.h"
#include "OledDisplay.h"
#include "Input.h"

// component instantiation
Buzzer buzzer;
OledDisplay oledDisplay;
Input input;

// system variables
int appMode = 0;
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

void setup()
{
  // initialize serial communications
  Serial.begin(115200);  

  // initialize buzzer system
  buzzer.init();
  
  // initialize display
  oledDisplay.init();

  // initialize input system
  input.init();

  // mute sounds for now
  buzzer.mute();

  // start main menu display
  changeAppMode(0);
}

void loop()
{
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
  oledDisplay.print(10, 1, "1. EMG");
  oledDisplay.print(10, 20, "2. ECG/EEG");
  oledDisplay.print(10, 40, "3. PULSE");
  
  buzzer.beep(200,600);
  buzzer.beep(300,200);
  buzzer.beep(400,300);
}

void mainMenuUpdate() {

  if (input.getButtonPress(1)) {

    changeAppMode(1);
    
  } else if (input.getButtonPress(2)) {

    changeAppMode(2);
    
  } else if (input.getButtonPress(3)) {
    
    changeAppMode(3);
  }

  delay(16);
}

// ################## PULSE ##################

void pulseStart() {

  // reset all sensor / display variables
  LastTime = 0;
  ThisTime;
  BPMTiming = false;
  BeatComplete = false;
  BPM = 0;
  highestValueAverage = 0.0f;
  lowestValueAverage = 1025.0f;
  valueAverage = 0.0f;
  newBPM = 0;
  topLimit = 0.0f;
  bottomLimit = 0.0f;
  timesBPMWasClose = 0;

  buzzer.beep(200,600);
  buzzer.beep(300,200);
  buzzer.beep(400,300);

  oledDisplay.clear(); 
  oledDisplay.print(1, 1, "PULSE");
}

void pulseUpdate() {

  // get data
  int bioData = input.getPulseData(false);

  // computer averages
  computeAverageValues(bioData);

  /*
  Serial.print(highestValueAverage);
  Serial.print(",");
  Serial.print(lowestValueAverage);
  Serial.print(",");
  Serial.print(topLimit);
  Serial.print(",");
  Serial.print(bottomLimit);
  Serial.print(",");
  Serial.print(bioData);
  Serial.print(",");
  */

  // dump average value to serial
  Serial.println(valueAverage); 

  // display raw graph of average value
  displayData(bioData, 30, 10);
  
  // actual pusle calculation
  // 
  // The idea here is, when the pulse drops under 25% of the 'average bottom' it's half a Beat.
  // When the pulse jumps above 75% of the 'average bottom' then it's a full beat.
  // Once we have a full beat get the amount of time it took, then figure out how many of
  // Those you could fit into a minute. Then you have 'beats per minute'. 
  // It seems pretty weak, to be honest. But this appears to be what others are doing.
  //
  // I added some code that forces the bpm to be within '10' of the last 3 readings before it counts.
  // This is supposed to be like a 'confidence' reading, and if it's too eratic, we don't even show it.

  // calcular limits for beat watching
  topLimit = lowestValueAverage + ((highestValueAverage - lowestValueAverage) * 0.75f);
  bottomLimit = lowestValueAverage + ((highestValueAverage - lowestValueAverage) * 0.25f);
  ThisTime = millis();

  if (bioData > topLimit) {

    if (BeatComplete) {

      newBPM = int(60 / (float(ThisTime - LastTime) / 1000));
      BPMTiming = false;
      BeatComplete = false;

      if (abs(newBPM - BPM) < 10)
        timesBPMWasClose ++;
      else
        timesBPMWasClose = 0;

      BPM = newBPM;

      if (timesBPMWasClose > 3 && BPM > 30 && BPM < 200) {
        
        buzzer.beep(12, 250);
  
        char bpmBuffer[5];
        itoa(BPM, bpmBuffer, 10);
        sprintf(outputBuffer, "%s BPM", bpmBuffer);
  
        oledDisplay.eraseRect(60, 0, 128, 10);
        oledDisplay.print(60, 1, outputBuffer);
      }
    }
    
    if (BPMTiming == false)
    {
      LastTime = millis();
      BPMTiming = true;
    }
  }
  
  if ((bioData < bottomLimit) & BPMTiming)
    BeatComplete = true;

  // if 4th button is held down, exit display
  if (input.getButtonPress(4))
    changeAppMode(0);
}

// ################## ECG ##################

void ECGStart() {

  highestValueAverage = 0.0f;
  lowestValueAverage = 1025.0f;
  valueAverage = 0.0f;

  buzzer.beep(200,600);
  buzzer.beep(300,200);
  buzzer.beep(400,300);

  oledDisplay.print(1, 1, "ECG");
}

void ECGUpdate() {

  // get data ( don't filter it )
  int bioData = input.getECGData(false);

  // computer averages
  computeAverageValues(bioData);

  // dump raw value to serial
  Serial.println(bioData); 

  // display graph of raw value
  displayData(bioData, 20, 10);

  // actual heartrate calculation

  // if 4th button is held down, exit display
  if (input.getButtonPress(4))
  changeAppMode(0);
}

// ################## EMG ##################

void EMGStart() {
  
  highestValueAverage = 0.0f;
  lowestValueAverage = 1025.0f;
  valueAverage = 0.0f;
  
  buzzer.beep(200,600);
  buzzer.beep(300,200);
  buzzer.beep(400,300);

  oledDisplay.print(1, 1, "EMG");
}

void EMGUpdate() {
  
  // filtering and averaging disabled here until it's better understood if they're a good use

  // get data
  int bioData = input.getEMGData(false);

  // computer averages
  computeAverageValues(bioData);

  // dump average value to serial
  Serial.println(bioData); 

  // display raw graph of average value
  //displayData(valueAverage, 20, 10);v
  displayData(bioData, 20, 10);

  // actual muscle flex percentage calculation

  // if 4th button is held down, exit display
  if (input.getButtonPress(4))
    changeAppMode(0);

  //delay(5);
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

    oledDisplay.eraseRect(0, 11, 128, 64);
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