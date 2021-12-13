/*
Copyright (c) 2015 Jack Berkhout

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

This is a library for the Adafruit Capacitive Touch Screens
  ----> http://www.adafruit.com/products/1947
This chipset uses I2C to communicate

Based on code by Limor Fried/Ladyada for Adafruit Industries.
MIT license, all text above must be included in any redistribution.

On the shield, these jumpers were bridged:
Int - #7 (default)
SCK - 13 (default)
SO  - 12 (default)
SI  - 11
SCL - SCL
SDA - SDA

Usage:

#include "mbed.h"
#include "SPI_TFT_ILI9341.h"
#include "FT6206.h"
#include "Arial12x12.h"

#define PIN_XP          A3
#define PIN_XM          A1
#define PIN_YP          A2
#define PIN_YM          A0
#define PIN_SCLK        D13
#define PIN_MISO        D12
#define PIN_MOSI        D11
#define PIN_CS_TFT      D10  // chip select pin
#define PIN_DC_TFT      D9   // data/command select pin.
#define PIN_RESET_TFT   D8
//#define PIN_BL_TFT      D7
#define PIN_CS_SD       D4

#define PORTRAIT        0
#define LANDSCAPE       1

#define PIN_SCL_FT6206  P0_28
#define PIN_SDA_FT6206  P0_27
#define PIN_INT_FT6206  D7

SPI_TFT_ILI9341 TFT(PIN_MOSI, PIN_MISO, PIN_SCLK, PIN_CS_TFT, PIN_RESET_TFT, PIN_DC_TFT, "TFT"); // mosi, miso, sclk, cs, reset, dc 
FT6206 FT6206(PIN_SDA_FT6206, PIN_SCL_FT6206, PIN_INT_FT6206); // sda, scl, int

int main()
{
    //Configure the display driver
    TFT.claim(stdout);
    TFT.background(Black);
    TFT.foreground(White);
    TFT.set_orientation(LANDSCAPE);
    TFT.cls();

    //Print a welcome message
    TFT.set_font((unsigned char*) Arial12x12);
    TFT.locate(0,0);
    TFT.printf("Hello mbed!\n");

    while(1) {
        int X1, Y1, X2, Y2;
        TS_Cap p;
        if (FT6206.getTouchPoint(p)) {
            X1 = X2;
            Y1 = Y2;
            X2 = p.x;
            Y2 = p.y;
            TFT.locate(0,12);
            printf("Touch %3d %3d\n", p.x, p.y);
            if ((X1 > 0) && (Y1 > 0) && (X2 > 0) && (Y2 > 0)) {
                TFT.line(X1, Y1, X2, Y2, RGB(255,128,255));
            }
        }
    }
}
*/

#ifndef FT6206_H
#define FT6206_H

#include "mbed.h"

#define FT6206_I2C_FREQUENCY      400000

#define FT6206_ADDR           0x38
#define FT6206_G_FT5201ID     0xA8
#define FT6206_REG_NUMTOUCHES 0x02

#define FT6206_NUM_X             0x33
#define FT6206_NUM_Y             0x34

#define FT6206_REG_MODE 0x00
#define FT6206_REG_CALIBRATE 0x02
#define FT6206_REG_WORKMODE 0x00
#define FT6206_REG_FACTORYMODE 0x40
#define FT6206_REG_THRESHHOLD 0x80
#define FT6206_REG_POINTRATE 0x88
#define FT6206_REG_FIRMVERS 0xA6
#define FT6206_REG_CHIPID 0xA3
#define FT6206_REG_VENDID 0xA8

// calibrated for Adafruit 2.8" ctp screen
#define FT6206_DEFAULT_THRESSHOLD 0x80

#define ILI9341_TFTWIDTH  320
#define ILI9341_TFTHEIGHT 240

class TS_Cap {
 public:
    TS_Cap(void);
    TS_Cap(int16_t x, int16_t y, int16_t z);
    
    bool operator==(TS_Cap);
    bool operator!=(TS_Cap);
    
    int16_t x, y, z;
};

class FT6206 {
  public:
    //!Creates an instance of the class.
    /*!
    FT6206 display touchscreen at I2C using I2C port pins sda, scl, and interrupt pins.
    TMP102
    \param sda, scl, interrupt
    */
    FT6206(PinName sda, PinName scl, PinName interrupt);

    /*!
    Destroys instance.
    */ 
    ~FT6206();

    bool init(uint8_t thresh = FT6206_DEFAULT_THRESSHOLD);  

    void writeRegister8(char reg, char val);
    char readRegister8(char reg);
    char dataReceived(void);

    void readData(uint16_t *x, uint16_t *y);
    
    bool touched(void);
    TS_Cap getPoint(void);
    TS_Cap clearPoint(void);

    void checkDataReceived(void);
    
    bool getDataReceived(void);

    void waitScreenTapped(void);

    bool getTouchPoint(TS_Cap &p);

    Ticker tick;

 private:
//    DigitalIn m_interrupt; 
    InterruptIn m_interrupt;

    I2C m_i2c;
    int m_addr;
    char data[2];

    uint8_t touches;
    uint16_t touchX[2], touchY[2], touchID[2];
    
    bool DataReceived;


};

#endif

