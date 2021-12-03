#include "TS.h"
bool TS_point::operator==(TS_point p) {
  return ((p.x == x) && (p.y == y));
}

bool TS_point::operator!=(TS_point p) {
  return ((p.x != x) || (p.y != y));
}

TouchScreen::TouchScreen(int pinYP, int pinYM, int pinXP, int pinXM, TS_config configuration):
                        pYP(pinYP), pYM(pinYM), pXP(pinXP), pXM(pinXM), conf(configuration){
    pxX= 1.0/(conf.TS_adc_Xmax - conf.TS_adc_Xmin);
    pxY = 1.0/(conf.TS_adc_Ymax - conf.TS_adc_Ymin); 
}

void TouchScreen::setOrientation(TS_Orientation orientation){
    ts_orient = orientation;
}


bool TouchScreen::getPoint(TS_point *point){
    bool valid = true;

    int xV = getCoords(setX); 
    int yV = getCoords(setY); 

    point->xV = xV;
    point->yV = yV;

    //if(xV < conf.TS_adc_Xmin || xV > conf.TS_adc_Xmax) valid = false;
    int x = ((1 - nrm_adcX(xV)) * conf.TFTWIDTH); 

    //if(yV < conf.TS_adc_Ymin || yV > conf.TS_adc_Ymax) valid = false;
    int y = ((1 - nrm_adcY(yV)) * conf.TFTHEIGHT);

    switch(ts_orient) {
        case TS_PORTRAITE:
            point->x = x;
            point->y = y;
            break;
        case TS_LANDSCAPE:
            point->x = y;
            point->y = conf.TFTWIDTH - x;
            break;
        case TS_PORTRAITE_REV:
            point->x = conf.TFTWIDTH - x;
            point->y = conf.TFTHEIGHT - y;
            break;
        case TS_LANDSCAPE_REV:
            point->x = conf.TFTHEIGHT - y;
            point->y = x;
            break;
        default:      
            point->x = x;
            point->y = y;
    }
    return valid;
}

int TouchScreen::getCoords(Axis axis){
    int value = 0;
    switch(axis){
        case setX:
            setDigitalPin(pXM,pOutput,0);   // XM = Output Low
            setDigitalPin(pXP,pOutput,1);   // XP = Output high
            setDigitalPin(pYM,pInput,0);    // YM = Input
            value = getADCvalue(pYP);       // YP = ADC
            //pc.printf("ADC YP value is %d\n", value);
            setDigitalPin(pXP,pOutput,0);   // XP = Output Low
            setDigitalPin(pYM,pOutput,0);   // YM = Output Low
            setDigitalPin(pYP,pOutput,0);   // YP = Output Low 
            break;
        case setY:
            setDigitalPin(pYM,pOutput,0);   // YM = Output Low
            setDigitalPin(pYP,pOutput,1);   // YP = Output high
            setDigitalPin(pXP,pInput,0);    // XP = Input
            value = getADCvalue(pXM);       // XM = ADC
            //pc.printf("ADC XM value is %d ", value);
            setDigitalPin(pYP,pOutput,0);   // YP = Output Low
            setDigitalPin(pXP,pOutput,0);   // XP = Output Low
            setDigitalPin(pXM,pOutput,0);   // XM = Output Low 
            break;
    }
    return value;
}

float TouchScreen::nrm_adcX(unsigned int x){
    return (x - conf.TS_adc_Xmin) * pxX;
}

float TouchScreen::nrm_adcY(unsigned int y){
    return (y - conf.TS_adc_Ymin) * pxY;
}

