/* TS library is for the 4-wire resistive Touch Screen (without an interface) 
*  which is usually included on TFT LCD like are MCUFriend displays.
*  The pins of the Touch Screen are shared with display interface and it is 
*  not possible to read the TouchScreen and write to the display at the "same time".
*  Also the TouchScreen coordinates must be read one by one and TouchScreen's pins must be
*  configurted for reading for one Axis (X) and then for second one (Y).
*
*  TS library mabe by Jan Kamidra 28.6.2020 under Mbed and its tools (Mbed Studio online).
*/

/** Example for testing 
* @code 

#include "mbed.h"
#include "TS.h"

#define YP A3
#define YM D9
#define XM A2
#define XP D8
#define RD A0

TS_config config = {2955,325,3070,470,240,320};     // that are minimum and maxim mV for all axis  
TouchScreen ts(YP,YM,XP,XM,config);

int main()
{
    printf("Example starting..\n");
    DigitalInOut(A0,PIN_OUTPUT,PushPullNoPull,1); //this line is necessary only without display driver
    DigitalOut led(LED1);
    TS_point point;
    //ts.setOrientation(TouchScreen::TS_PORTRAITE);

    while (true){
        if(ts.getPoint(&point)) printf("X: %d, Y: %d\n",point.x,point.y);
        led = !led;
        thread_sleep_for(1000);
    }
}
* @/code
*/

#ifndef TS_H
#define TS_H

#include "TS_porting.h"

struct TS_point{
    /** Coordinates for XY axis calculated for selected orientation*/
    int x, y;

    /** Coordinates in mV for XY plates of the TouchScreen (orientation is not included)*/
    int xV, yV;

    /** Check if the current point is equal to another point
    *
    * @param p The other point being checked for equivalence
    * @return  return true according to whether the points are equal else false
    */
    bool operator==(TS_point);

    /** Check if the current point is not equal to another point
    *
    * @param p The other point being checked for equivalence
    * @return  return false according to whether the points are not equal else true
    */
    bool operator!=(TS_point);
};



struct TS_config{
   unsigned int TS_adc_Xmax,    // max value for X in mV
                TS_adc_Xmin,    // max value for X in mV
                TS_adc_Ymax,    // max value for Y in mV
                TS_adc_Ymin,    // min value for Y in mV
                TFTWIDTH,       // Display width
                TFTHEIGHT;      // Display height
};


class TouchScreen: protected TouchScreenPort {
    public:
        enum TS_Orientation{
            TS_PORTRAITE        = 0,
            TS_LANDSCAPE        = 1,
            TS_PORTRAITE_REV    = 2,
            TS_LANDSCAPE_REV    = 3
        }; 

        /** Constructor of a new Touch Screen object
        *
        * @param pinYP      Y+ pin of Y plate
        * @param pinYM      Y- pin of Y plate
        * @param pinXP      X+ pin of X plate
        * @param pinXM      M- pin of X plate
        * @param config     Necessary configuration via TS_config struct
        */
        TouchScreen(int pinYP,int pinYM, int pinXP, int pinXM, TS_config configuration);

        /** Set the orientation 
        *
        * @param orientation    It is set via the TS_Orientation enum
        *                       TS_PORTRAITE(default), TS_LANDSCAPE,TS_PORTRAITE_REV and TS_LANDSCAPE_REV 
        */
        void setOrientation(TS_Orientation orientation);

        /** Get coordinates XY via struct TS_point
        *
        * @param point  pointer to the TS_point where coordinates will be stored.
        * @return       return true if data are valid, else false.
        *               That mean if data are in range of TouchScren plates defined in TS_config.
        */
        bool getPoint(TS_point *point);


    private:
        enum Axis{
            setX    = 0,
            setY    = 1
        };
        enum Mode{
            pInput  = 0,
            pOutput = 1
        };
        int             pYP, pYM, pXP, pXM;
        TS_config       conf;
        TS_Orientation  ts_orient;//   = TS_PORTRAITE;
        float           pxX, pxY;
        /** Pins setting for measuring on the plates (X/Y) or for releasing pins for further use.*/
        int     getCoords(Axis axis);
        /** ADC X value normalization*/
        float   nrm_adcX(unsigned int x);
        /** ADC Y value normalization*/
        float   nrm_adcY(unsigned int y);
};
#endif