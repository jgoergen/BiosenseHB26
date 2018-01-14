// ################## PULSE ##################

void pulseStart() {
   modeName = "PULSE";
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
  oledDisplay.print(1, 1, modeName);
  oledDisplay.display();
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