/*
* Mbed implementation but teory is it can be used also on another platform, I hope.
*/

#ifndef TS_PORTING_H
#define TS_PORTING_H

#include "mbed.h"

class TouchScreenPort{
    protected:
    int getADCvalue(int pin);
    void setDigitalPin(int pin, int dir, int value);
};
#endif