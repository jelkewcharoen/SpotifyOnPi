/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

/******************************************************************************
 * Tested on NUCLEO-F401RE, LPCXpresso11U68, LPCXpresso824-MAX platform.
 *****************************************************************************/

#include "mbed.h"

#include "DmTftHX8353C.h"
#include "DmTftS6D0164.h"
#include "DmTftIli9325.h"
#include "DmTftIli9341.h"
#include "DmTftSsd2119.h"
#include "DmTftRa8875.h"
#include "DmTouch.h"
#include "DmTpFt6x06.h"

//#include "TMP36.h"
#include "DHT.h"
//resistive touch code
#include "TS.h"
#define YP p20
#define YM p18
#define XM p17
#define XP p19
//TS_config config = {2955,325,3070,470,320,240}; //was 240 320
TS_config config = {2750,1000,2350,400,320,240};     // that are minimum and maxim mV for all axis  
TouchScreen ts(YP,YM,XP,XM,config);

DHT humidity(p16, DHT11);
/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

/* Note that there are restrictions on which platforms that can use printf
   in combinations with the DmTftLibrary. Some platforms (e.g. LPC1549 LPCXpresso)
   use the same pins for USBRX/USBTX and display control. Printing will
   cause the display to not work. Read more about this on the display's notebook
   page. */
//#define log(...) printf(__VA_ARGS__)
#define log(...)

Serial pc(USBTX, USBRX);
//attempt to communicate with the Pi
RawSerial  pi(USBTX, USBRX);
/******************************************************************************
 * Local variables
 *****************************************************************************/
 
/*********  TFT DISPLAY INIT *********/
DmTftIli9341 tft(p8, p10, p5, p6, p7); 

/******************************************************************************
 * Global variables
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/


/******************************************************************************
 * Main
 *****************************************************************************/

int main() {
  log("init tft \r\n");
  tft.init();
  uint16_t w = tft.width();
  uint16_t h = tft.height();
  
  pi.baud(9600);
  //tft.drawString(20, 20, "x:");
  //tft.drawString(80, 20, "y:");
  
  //temperature and humidity readings go here
  tft.drawString(100, 100, "Singing in the Shower Assistant");
  float temp, hum;
 
  while(temp < 88.0 && hum < 70.0) { 
    if (humidity.readData() != ERROR_NONE) {
        pc.printf("There was an error reading the sensor");
        }
    hum = humidity.ReadHumidity();
    temp = humidity.ReadTemperature(FARENHEIT);
    pc.printf("Temp is %f and humidity is %f", temp, hum);
    wait(.5); 
    } 
  
  tft.clearScreen();
  tft.drawString(150, 20, "Spotify");
  tft.drawRectangle(20, 50, 110, 150, RED);
  tft.drawRectangle(120, 50, 220, 150, RED);
  tft.drawRectangle(20, 200, 110, 295, RED);
  tft.drawRectangle(120, 200, 220, 295, RED);
  tft.drawString(25, 55, "Skip");
  tft.drawString(125, 55, "Decrease");
  tft.drawString(25, 205, "Increase");
  tft.drawString(125, 205, "Pause");
  wait(1);
    
  //pc.printf("Example starting..\n");
  ts.setOrientation(TouchScreen::TS_PORTRAITE);
  TS_point point;
    
  while (1) {
    if (ts.getPoint(&point)) {
      //pc.printf("X: %d, Y: %d, ADC: %d\n",point.x,point.y, (AnalogIn((PinName)p20).read() * 3300));
      //pc.printf("X: %d, Y: %d, ADC: %d\n",point.x,point.y, (AnalogIn((PinName)p19).read() * 3300));
      //pc.printf("X: %d, Y: %d, ADC: %d\n",point.x,point.y, (AnalogIn((PinName)p18).read() * 3300));
      //pc.printf("X: %d, Y: %d, ADC: %d\n",point.x,point.y, (AnalogIn((PinName)p17).read() * 3300));
      tft.drawNumber(40, 20, point.x, 5, false);
      tft.drawNumber(120, 20, point.y, 5, false);
      tft.drawPoint(point.x, point.y, 2);
      if(point.x < 180 && point.x > 130 && point.y < 40 && point.y > 8) {
            pc.printf("we selected skip");
            tft.drawString(20, 295, "Selected the Skip Option");
            pi.putc('S');
      }
      if(point.x < 320 && point.x > 185 && point.y < 150 && point.y > 100) {
            pc.printf("we selected pause");
            tft.drawString(20, 295, "Selected the Pause Option");
            pi.putc('P');
      }
      }
    wait(1);
  }
}
