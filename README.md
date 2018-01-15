# BiosenseHB26
An opensource firmware for the Hackerbox #0026 Biosense Kit!

## Current developement progress:

| Progress        | Description           
| ------------- |:-------------:
| Done! | Remove Game from example firmware. |
| Done! | Change display library to adafruit version. |
| Done! | Break firmware into multiple classes. |
| 80% | Filter signal better before relaying to host or graphing. |
| 0% | Add external SD Card Module support. |
| 50% | Improve on device graphing. |
| 0% | Add on device configuration via menus. |
| 50% | Correlate pulse data to BPM on device. |
| 0% | Correlate EKC metrics on device. |
| 0% | Correlate EMG muscle contraction percentage on device. |


##Deeper notes on some of the TODOs:
###Filter signal better before relaying to host or graphing.
- I'm currently playing with running averaging and low pass filtering, is there a better way?
- My averaging attempts to be self adjusting over time, although it may be too aggressive. I need to find a better balance for it.
###Add external SD Card Module support.
- I'm guessing we would need to not only log the data but also millisecond markers for correlation? Maybe just a symbol every second or something similar?
- The ability to replay files as if the data was coming from the device naturally would be cool for interacting with external hosts
###Improve on device graphing.
- X axis zooming would be nice.
- Could we find some inspiration for improvement here? https://github.com/laurb9/tiny_scope
###Some links I've saved but not necessarily gone through yet on data correlation on device
- Pulse BPM: http://www.xtronical.com/basics/heart-beat-sensor-ecg-display/
- EKG Data: https://www.wikihow.com/Calculate-Heart-Rate-from-ECG
- EKG Data: https://courses.kcumb.edu/physio/ecg%20primer/normecgcalcs.htm
- EKG Data: https://imotions.com/blog/heart-rate-variability/


##Additional information:
###Learn more about electronics
- https://www.instructables.com/id/Muscle-EMG-Sensor-for-a-Microcontroller/
- https://www.instructables.com/id/DIY-EEG-and-ECG-Circuit/

###good desktop app for recieving usb serial data from device?
- http://www.shifz.org/brainbay/

##FAQ:
"It says I'm missing filters.h when compiling, am I missing a library?" Yep! You can find it here: https://github.com/JonHub/Filters 
