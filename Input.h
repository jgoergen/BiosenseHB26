#include "Arduino.h"

#ifndef Input_h
#define Input_h

class Input
{
    public:
        Input();
        void init();
        bool getButtonPress(int which);
        int getPulseData(bool filtered);
        int getECGData(bool filtered);
        int getEMGData(bool filtered);

    private:
        int reading;
};

#endif