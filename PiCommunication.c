#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
 
int main(int argc, char ** argv) {
  int fd;
  char buf[256];
  int n;
  // Open the Port. We want read/write, no "controlling tty" status, 
  // and open it no matter what state DCD is in
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);  
  //open mbed's USB virtual com port
  if (fd == -1) {  
    perror("open_port: Unable to open /dev/ttyACM0 - ");
    return(-1);
  }
  // Turn off blocking for reads, use (fd, F_SETFL, FNDELAY) if you want that
  fcntl(fd, F_SETFL, 0);
  //Linux Raw serial setup options
  struct termios options; 
  tcgetattr(fd,&options);   //Get current serial settings in structure
  cfsetspeed(&options, B9600);   //Change a only few
  options.c_cflag &= ~CSTOPB;
  options.c_cflag |= CLOCAL;
  options.c_cflag |= CREAD;
  cfmakeraw(&options);
  tcsetattr(fd,TCSANOW,&options);    //Set serial to new settings
  sleep(1);
  // Write to the port
  /*
  n = write(fd,"Hello mbed\r",11);
  if (n < 0) {
    perror("Write failed - ");
    return -1;
  }
  sleep(2);
  */

  // Read the characters from the port if they are there
  while (1) {
    n = read(fd, buf, 2); //only read the most recent character (last will be made a zero?)
    if (n < 0) {
        perror("Read failed - ");
        return -1;
    } else if (n == 0) printf("No data on port\n");
    else {
        buf[n] = 0;
        printf("%i bytes read back: %s\n\r", n, buf);
    }
    if (buf == "S") {
        //we want to skip the next song
    } else if (buf == "P") {
        //we want to pause the song
    }
    //other potential capabilites: choose a playlist, choose a song from a subset

    sleep(1000); //one second of waiting to align with mbed timing
  }
  sleep(1);
  /*
  //Send command to blink mbed led 10 times at one second rate
  //mbed code turns on led2 with a '1' and off with a '0'
  //mbed echoes back each character
  buf[1] = '\r'; // end of line
  buf[2] = 0; // end of string
  for(int i=0;i<10;i++) {
      write(fd,"1",1);        //led on
      sleep(1);               //one second delay  
      read(fd,buf,1); //read echo character
      printf("%s\n\r",buf);   //print in terminal window
      write(fd,"0",1);        //led off
      sleep(1);   
      read(fd,buf,1);
      printf("%s\n\r",buf);
  }
  */
  // Don't forget to clean up and close the port
  tcdrain(fd);
  close(fd);
  return 0;
}