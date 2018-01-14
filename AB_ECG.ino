// ################## ECG ##################

void ECGStart() {
   modeName = "ECG";
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


void ECGUpdate() {

  
  int bioData = input.getECGData(false); // get data ( don't filter it )

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
