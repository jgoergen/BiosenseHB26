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

// sensor polling and graphing variables
float highestValueAverage = 0.0f;
float lowestValueAverage = 1025.0f;
float valueAverage = 0.0f;
float height = 0.0f;
float scale = 1; 
float mappedVal = 0.0f;
int x = 0; 
int LastTime = 0;
int ThisTime;
bool BPMTiming = false;
bool BeatComplete = false;
int averageBPM = 0;
int BPM = 0;
char outputBuffer[55]; 
float framesSinceLastLowest = 0;
float framesSinceLastHighest = 0;

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
      pulseUpdate();
      break;

    case 2:
      ECGUpdate();
      break;

    case 3:
      EMGUpdate();
      break;
  }

  oledDisplay.display();
}

void changeAppMode(int mode) {

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

  appMode = mode;
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

  buzzer.beep(200,600);
  buzzer.beep(300,200);
  buzzer.beep(400,300);

  oledDisplay.clear(); 
  oledDisplay.print(1, 1, "PULSE");
}

void pulseUpdate() {

  // get data
  int bioData = input.getPulseData();

  // computer averages
  computeAverageValues(bioData);

  // dump average value to serial
  Serial.println(valueAverage); 

  // display raw graph of average value
  displayData(valueAverage, 20, 10);
  
  // actual pusle calculation  
  float midLine = lowestValueAverage + ((highestValueAverage - lowestValueAverage) / 2);
  ThisTime = millis();
  
  if (valueAverage > midLine) {

    if (BeatComplete) {

      BPM = int(60 / (float(ThisTime - LastTime) / 1000));
      BPMTiming = false;
      BeatComplete = false;
      averageBPM = (averageBPM + BPM) / 2;

      if (averageBPM > 30 && averageBPM < 200) {
        
        buzzer.beep(12, 250);
  
        char bpmBuffer[5];
        itoa(averageBPM, bpmBuffer, 10);
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
  
  if ((valueAverage < midLine) & BPMTiming)
    BeatComplete = true;

  /*
  Serial.print(highestValueAverage);
  Serial.print(",");
  Serial.print(lowestValueAverage);
  Serial.print(",");
  Serial.print(midLine);
  Serial.print(",");
  Serial.println(valueAverage);
  */
  
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

  // get data
  int bioData = input.getECGData();

  // computer averages
  computeAverageValues(bioData);

  // dump average value to serial
  Serial.println(valueAverage); 

  // display raw graph of average value
  displayData(valueAverage, 20, 10);

  // actual heartrate calculation

  // if 4th button is held down, exit display
  if (input.getButtonPress(4))
  changeAppMode(0);

  delay(5);
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
  
  // get data
  int bioData = input.getEMGData();

  // computer averages
  computeAverageValues(bioData);

  // dump average value to serial
  Serial.println(valueAverage); 

  // display raw graph of average value
  displayData(valueAverage, 20, 10);

  // actual muscle flex percentage calculation

  // if 4th button is held down, exit display
  if (input.getButtonPress(4))
    changeAppMode(0);

  //delay(5);
}

// TODO: This should probably be part of the input class as just properties. The get data functions would compute them when polled
void computeAverageValues(int value) {

  valueAverage = (valueAverage + value) / 2.0f;  
  
  if (valueAverage > highestValueAverage) {

    highestValueAverage = (highestValueAverage + valueAverage) / 2.0f; 
    framesSinceLastHighest = 0;

  } else {

    framesSinceLastHighest ++;
    highestValueAverage -= 0.15f;
  }
  
  if (valueAverage < lowestValueAverage) {

    lowestValueAverage = (lowestValueAverage + valueAverage) / 2.0f; 
    framesSinceLastLowest = 0;

  } else {

    framesSinceLastLowest ++;
    lowestValueAverage += 0.08f;
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

    oledDisplay.clear();  
    x = 0;
  }
  
  int graphHeight = 64 - topPadding - bottomPadding;
  mappedVal = (int)map(valueAverage, lowestValueAverage, highestValueAverage, 0, graphHeight);
  oledDisplay.drawLine(x, (graphHeight / 2) + topPadding, x, topPadding + (mappedVal - (graphHeight / 2))+ (graphHeight / 2) - 1);
  x ++;  
}