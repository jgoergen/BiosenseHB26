// ################## EMG ##################

void EMGStart() {
   modeName = "EMG";
  highestValueAverage = 0.0f;
  lowestValueAverage = 1025.0f;
  valueAverage = 0.0f;
  
  buzzer.beep(200,600);
  buzzer.beep(300,200);
  buzzer.beep(400,300);
  
  oledDisplay.clear(); // clear menu before displaying next thing
  oledDisplay.print(1, 1, modeName);
  oledDisplay.display();
}

void EMGUpdate() {
  
  // filtering and averaging disabled here until it's better understood if they're a good use

  int bioData = input.getEMGData(false);  // get data
  computeAverageValues(bioData);  // computer averages

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


