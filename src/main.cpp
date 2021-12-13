
#include "mbed.h"

#include "DmTftIli9341.h"
#include "DmTouch.h"
#include "DHT.h"
#include "FT6206.h"

DigitalOut skip(p22);
DigitalOut pause(p14);
DigitalOut play(p24);
DigitalOut playlist(p25);
DigitalOut decrease(p26);
DigitalOut increase(p23);

DHT humidity(p16, DHT11);

#define log(...)

Serial pc(USBTX, USBRX);
 
/*********  TFT DISPLAY INIT *********/
DmTftIli9341 tft(p8, p10, p5, p6, p7); 

FT6206 screen(p28, p27, p21);

extern uint8_t dmlogo[];


/******************************************************************************
 * Functions
 *****************************************************************************/

void draw_logo() {
  tft.fillRectangle(0, 0, 240, 39, 0x1EAC);
  tft.drawImage(100, 0, 40, 40, (uint16_t *) dmlogo);
  tft.fillRectangle(0, 0, 240, 1, 0x1EAC);
  
  tft.setTextColor(0x1EAC, WHITE);
  tft.drawString(150, 15, "Spotify");
  tft.setTextColor(BLACK, WHITE);
}

void draw_default_options() {
  tft.drawRectangle(20, 50, 110, 125, 0x1EAC);
  tft.drawRectangle(130, 50, 220, 125, 0x1EAC);
  tft.drawRectangle(20, 135, 110, 210, 0x1EAC);
  tft.drawRectangle(130, 135, 220, 210, 0x1EAC);
  tft.drawRectangle(20, 215, 110, 290, 0x1EAC);
  tft.drawRectangle(130, 215, 220, 290, 0x1EAC);
  tft.setTextColor(BLACK, WHITE);
  tft.drawString(25, 55, "Play");
  tft.drawString(135, 55, "Pause");
  tft.drawString(25, 140, "Increase");
  tft.drawString(25, 155, "Volume");
  tft.drawString(135, 140, "Decrease");
  tft.drawString(135, 155, "Volume");
  tft.drawString(25, 220, "Skip");
  tft.drawString(135, 220, "Playlist");
}
    
int main() {
  log("init tft \r\n");
  tft.init();
  uint16_t w = tft.width();
  uint16_t h = tft.height();
  
  pc.baud(115200);
  
  play = 0;
  pause = 0;
  increase = 0;
  decrease = 0;
  skip = 0;
  playlist = 0;
  
  tft.drawString(88, 90, "Singing");
  tft.drawString(90, 110, "in the");
  tft.drawString(89, 130, "Shower");
  tft.drawString(80, 150, "Assistant");
  float temp = 0.0;
  float hum = 0.0;
  while(temp < 80.0 && hum < 50.0) { 
    if (humidity.readData() != ERROR_NONE) {
        //pc.printf("There was an error reading the sensor");
        }
    hum = humidity.ReadHumidity();
    temp = humidity.ReadTemperature(FARENHEIT);
    //pc.printf("Temp is %f and humidity is %f", temp, hum);
    wait(.5); 
    } 
  tft.clearScreen();
  draw_logo();

  draw_default_options();
  
  wait(1);
  TS_Cap point;
  while (1) {
    
    if (screen.getTouchPoint(point)) {

      if(point.x < 125 && point.x > 50 && point.y < 110 && point.y > 20) { //pause
          pause = !pause;
          tft.setTextColor(0x1EAC, WHITE);
          tft.drawString(20, 15, "Pause"); //print out in White
          wait(1);
          tft.setTextColor(0x1EAC, 0x1EAC);
          tft.drawString(20, 15, "Pause"); //print out in Green
            
      } else if(point.x < 125 && point.x > 50 && point.y < 220 && point.y > 130){ //play
          play = !play;
          tft.setTextColor(0x1EAC, WHITE);
          tft.drawString(20, 15, "Play"); //print out in White
          wait(1);
          tft.setTextColor(0x1EAC, 0x1EAC);
          tft.drawString(20, 15, "Play"); //print out in Green
      } else if(point.x < 210 && point.x > 135 && point.y < 110 && point.y > 20) { //decrease
          decrease = !decrease;
          tft.setTextColor(0x1EAC, WHITE);
          tft.drawString(20, 15, "- Volume"); //print out in White
          wait(1);
          tft.setTextColor(0x1EAC, 0x1EAC);
          tft.drawString(20, 15, "- Volume"); //print out in Green
      } else if(point.x < 210 && point.x > 135 && point.y < 220 && point.y > 130) { //increase
          increase = !increase;
          tft.setTextColor(0x1EAC, WHITE);
          tft.drawString(20, 15, "+ Volume"); //print out in White
          wait(1);
          tft.setTextColor(0x1EAC, 0x1EAC);
          tft.drawString(20, 15, " + Volume"); //print out in Green
      } else if(point.x < 290 && point.x > 215 && point.y < 220 && point.y > 130) { //skip
          skip = !skip;
          tft.setTextColor(0x1EAC, WHITE);
          tft.drawString(20, 15, "Skip"); //print out in White
          wait(1);
          tft.setTextColor(0x1EAC, 0x1EAC);
          tft.drawString(20, 15, "Skip"); //print out in Green
      } else if(point.x < 290 && point.x > 215 && point.y < 110 && point.y > 20){ //playlist
          playlist = !playlist;
          tft.setTextColor(0x1EAC, WHITE);
          tft.drawString(20, 15, "Playlist"); //print out in White
          wait(1);
          tft.setTextColor(0x1EAC, 0x1EAC);
          tft.drawString(20, 15, "Playlist"); //print out in Green
          char str[128];
          tft.setTextColor(BLACK,WHITE);
          while (!pc.readable()) {
              }
            
          pc.gets(str, 40);
          wait(.1);
          tft.clearScreen();
          draw_logo();
          wait(.25);
          tft.drawRectangle(20, 50, 110, 145, 0x1EAC);
          tft.drawRectangle(130, 50, 220, 145, 0x1EAC);
          tft.drawRectangle(20, 200, 110, 295, 0x1EAC);
          tft.drawRectangle(130, 200, 220, 295, 0x1EAC);
          tft.setTextColor(BLACK,WHITE);
          //now need a way to divide up the string into parts
          char * token = strtok(str, "9");
          // loop through the string to extract all other tokens
          const char * one;
          const char * two;
          const char * three;
          char * four;
          int i = 0;
          while( token != NULL ) {
            if (i == 0) one = token;
            if (i == 1) two = token;
            if (i == 2) three = token;
            if (i == 3) four = token;
            token = strtok(NULL, "9");
            i++;
          }
          //remove newline character from end of last playlist name
          int size = strlen(four); //Total size of string
          four[size-1] = '\0'; //remove last character
          const char * new_four = four;
          
          if (i > 0) tft.drawString(25, 55, one);
          if (i > 1) tft.drawString(135, 55, two);
          if (i > 2) tft.drawString(25, 205, three);
          if (i > 3) tft.drawString(135, 205, new_four);
          
    top:
          while (!screen.getTouchPoint(point)) {  
              }
         bool selected = false;
         if(point.x < 150 && point.x > 50 && point.y < 230 && point.y > 150) { //selected playlist one
              tft.setTextColor(0x1EAC, WHITE);
              tft.drawString(20, 15, one); //print out in White
              wait(1);
              tft.setTextColor(0x1EAC, 0x1EAC);
              tft.drawString(20, 15, one); //print out in Green
              pc.printf("%s\n", one);
              selected = true;
         } else if(point.x < 295 && point.x > 200 && point.y < 110 && point.y > 20){ //selected playlist four
              tft.setTextColor(0x1EAC, WHITE);
              tft.drawString(20, 15, new_four); //print out in White
              wait(1);
              tft.setTextColor(0x1EAC, 0x1EAC);
              tft.drawString(20, 15, new_four); //print out in Green
              pc.printf("%s\n", new_four);
              selected = true; 
         } else if(point.x < 150 && point.x > 50 && point.y < 110 && point.y > 20) { //selected playlist two
              tft.setTextColor(0x1EAC, WHITE);
              tft.drawString(20, 15, two); //print out in White
              wait(1);
              tft.setTextColor(0x1EAC, 0x1EAC);
              tft.drawString(20, 15, two); //print out in Green
              pc.printf("%s\n", two);
              selected = true;
         } else if(point.x < 295 && point.x > 200 && point.y < 230 && point.y > 150){ //selected playlist three
              tft.setTextColor(0x1EAC, WHITE);
              tft.drawString(20, 15, three); //print out in White
              wait(1);
              tft.setTextColor(0x1EAC, 0x1EAC);
              tft.drawString(20, 15, three); //print out in Green
              pc.printf("%s\n", three);
              selected = true;
         }
         if (selected) { //we selected a playlist - now jump to it
             tft.clearScreen();
             draw_logo();
             draw_default_options();
         } else { //no playlist selected - keep waiting for one to be choosen
             goto top;
         }
          
      }
    wait(1);
  }
}
}
