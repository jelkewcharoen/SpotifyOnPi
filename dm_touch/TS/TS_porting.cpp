#include "TS_porting.h"

int TouchScreenPort::getADCvalue(int pin){
    return AnalogIn((PinName)pin).read() * 3300;
}

void TouchScreenPort::setDigitalPin(int pin, int dir, int value){
    PinDirection pDirection;
    if(dir == 0) pDirection = PIN_INPUT;
    else  pDirection = PIN_OUTPUT;
    DigitalInOut((PinName)pin,pDirection,PullDefault,value);
}
