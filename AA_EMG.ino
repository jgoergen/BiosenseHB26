// ################## EMG ##################

void EMGStart() {

  modeName = "EMG";

  // reset values
  highestValueAverage = 0.0f;
  lowestValueAverage = 1025.0f;
  valueAverage = 0.0f;

  buzzer.beep(200,600);
  buzzer.beep(300,200);
  buzzer.beep(400,300);

  // clear menu before displaying next thing
  oledDisplay.clear(); 
  oledDisplay.print(1, 1, modeName);
  oledDisplay.display();
}

void EMGUpdate() {
  
  // filtering and averaging disabled here until it's better understood if they're a good use

  // get data ( don't filter it )
  int bioData = input.getEMGData(false);  

  // computer averages
  computeAverageValues(bioData);  

  Serial.print(bioData);
  Serial.print(",");
  Serial.print(valueAverage); 
  Serial.println("|");

  // display raw graph of average value
  displayData(bioData, 10, 0);

  // actual muscle flex percentage calculation

  // if 4th button is held down, exit display
  if (input.getButtonPress(4))
    changeAppMode(0);

  delay(5);
}


