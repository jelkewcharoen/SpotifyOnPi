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

#include "FT6206.h"

FT6206::FT6206(PinName sda, PinName scl, PinName interrupt) : m_i2c(sda, scl), m_interrupt(interrupt)
{
    m_addr = (FT6206_ADDR << 1);
    m_i2c.frequency(FT6206_I2C_FREQUENCY);

    m_interrupt.mode(PullUp);
    m_interrupt.enable_irq(); 
    m_interrupt.fall(this, &FT6206::checkDataReceived);

    init();

//    tick.attach(this, &FT6206::checkDataReceived, 0.00001); // Every 10 us id data was received

}

FT6206::~FT6206()
{

}

/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
bool FT6206::init(uint8_t threshhold) {
  // change threshhold to be higher/lower
  writeRegister8(FT6206_REG_THRESHHOLD, FT6206_DEFAULT_THRESSHOLD);
//  writeRegister8(FT6206_REG_POINTRATE, 80);
  
  if (readRegister8(FT6206_REG_VENDID) != 17)
    return false;
  
  if (readRegister8(FT6206_REG_CHIPID) != 6)
    return false;

//  printf("Vend ID: %d\n", readRegister8(FT6206_REG_VENDID));
//  printf("Chip ID: %d\n", readRegister8(FT6206_REG_CHIPID));
//  printf("Firm V: %d\n", readRegister8(FT6206_REG_FIRMVERS));
//  printf("Rate Hz: %d\n", readRegister8(FT6206_REG_POINTRATE));
//  printf("Thresh: %d\n", readRegister8(FT6206_REG_THRESHHOLD));

  // dump all registers
/*
    for (int16_t i=0; i<0x20; i++) {
        printf("I2C $%02x = 0x %02x\n", i, readRegister8(i));
    }
*/
    return true;
}

bool FT6206::touched(void) {
    uint8_t n = readRegister8(FT6206_REG_NUMTOUCHES);
    if ((n == 1) || (n == 2))
        return true;
    return false;
}

void FT6206::checkDataReceived(void) {
    // InterruptIn pin used
//    // Runs every 0.00001 Sec. in Ticker
//    if (dataReceived()) {
        DataReceived = true;
//    }
}

bool FT6206::getDataReceived(void) {
    bool _DataReceived = DataReceived;
    DataReceived = false;
    return _DataReceived;
}

void FT6206::waitScreenTapped(void) {
    DataReceived = false;
    while (!getDataReceived()) {
        wait(0.01);
    }
}

bool FT6206::getTouchPoint(TS_Cap &p) {
//    if (touched()) {
//    if (dataReceived()) {
    if (getDataReceived()) {
        // Retrieve a point  
        p = getPoint();
        return true;
    }
    p = clearPoint();
    return false;   
}

/*****************************/

void FT6206::readData(uint16_t *x, uint16_t *y) {
    char i2cdat[16];

    m_i2c.write(m_addr, 0x00, 1);
    m_i2c.read(m_addr, i2cdat, 16);

/*
    for (int16_t i=0; i<0x20; i++) {
        printf("I2C %02x = %02x\n", i, i2cdat[i]);;
    }
*/

    touches = i2cdat[0x02];
    
    // printf("touches: %d\n", touches);
    if (touches > 2) {
        touches = 0;
        *x = *y = 0;
    }
    if (touches == 0) {
        *x = *y = 0;
        return;
    }

/*
    if (touches == 2) Serial.print('2');
    for (uint8_t i=0; i<16; i++) {
        printf("%02x ", i2cdat[i]);
    }
    printf("\n");
*/

/*
    printf("\n");
    if (i2cdat[0x01] != 0x00) {
        printf("Gesture #%d\n", i2cdat[0x01]); 
    }
*/

    //printf("# Touches: %d", touches);
    for (uint8_t i=0; i<2; i++) {
      touchY[i] = i2cdat[0x03 + i*6] & 0x0F;
      touchY[i] <<= 8;
      touchY[i] |= i2cdat[0x04 + i*6]; 
      touchX[i] = i2cdat[0x05 + i*6] & 0x0F;
      touchX[i] <<= 8;
      touchX[i] |= i2cdat[0x06 + i*6];
      touchID[i] = i2cdat[0x05 + i*6] >> 4;
    }
/*
    for (uint8_t i=0; i<touches; i++) {
      printf("ID #%d (%d, %d", touchID[i], touchX[i], touchY[i]);
      printf(")\n");
    }
*/
    *x = ILI9341_TFTWIDTH - touchX[0];
    *y = touchY[0];
}

TS_Cap FT6206::getPoint(void) {
    uint16_t x, y;
    readData(&x, &y);
    return TS_Cap(x, y, 1);
}

TS_Cap FT6206::clearPoint(void) {
    uint16_t x = 0, y = 0;
    return TS_Cap(x, y, 1);
}

char FT6206::readRegister8(char reg) {
    char val;
    m_i2c.write(m_addr, &reg, 1);
    m_i2c.read(m_addr, &val, 1);
    return val;
}

void FT6206::writeRegister8(char reg, char val) {
    char data[2];
    data[0] = reg;
    data[1] = val;
    m_i2c.write((int)FT6206_ADDR, data, 2);
}

char FT6206::dataReceived(void) {
    return !m_interrupt;
}

/****************/

TS_Cap::TS_Cap(void) {
    x = y = 0;
}

TS_Cap::TS_Cap(int16_t x0, int16_t y0, int16_t z0) {
    x = x0;
    y = y0;
    z = z0;
}

bool TS_Cap::operator==(TS_Cap p1) {
    return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

bool TS_Cap::operator!=(TS_Cap p1) {
    return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}
