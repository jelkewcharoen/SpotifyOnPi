# Singing in the Shower Assistant: ECE4180 Final Project

Group Members: Anna Carow and Jel Kewcharoen

## Concept

Singing in the Shower is a product that allows users to control the music playing on their Spotify account through a bluetooth speaker using a waterproof touchscreen. When you turn the shower on, the humidity and temperature sensor in the device will cause the screen to light up and prompt you if you want to begin playing music. From the touchscreen, you will have the option to start, pause, skip, change the volume, and change the playlist. The touchscreen will send signals to the Rasberry Pi, which is connected to both spotify and a bluetooth speaker. 

## Parts List

* 2.8" Adafruit TFT LCD with Touchscreen with ILI9341 Controller and capacative touch sensing
<img src="https://cdn-shop.adafruit.com/970x728/1770-00.jpg" width="200" height="150">
* DHT11 Humidity Sensor
* Rasberry Pi 3
* ARM mBed Microcontroller
* Bluetooth Speaker

## Software

* mbed DmTFT Libaray to support the ILI9341 Screen Controller
* mbed FT6026 Libaray to support the FT6026 capacative touchscreen controller 
* mbed DHT Libaray to support the DHT11 Humidity Sensor
* Spotify API/GPIO Libaray
* RPC Stuff?


## Building the System

To begin seeting up the mbed side of the system, import the DmTFT Library (https://os.mbed.com/users/displaymodule/code/DmTftLibrary/) into your project. This library gives the you availity to interface with a variety of different TFT LCD Libraries. For the screen used in this project, be sure the include the  DmTftIli9341.h header file. This library provides users with a variety of commands to draw on and print to the screen.


| mbed | TFT LCD |
| --- | --- |
| p1  | X |


A second library, FT6026, is needed to make use the capacative touch capabilites of the LCD screen. Import the mbed FT6026 library (https://os.mbed.com/users/JackB/code/FT6206/file/35e21af6733c/FT6206.cpp/). After than is complete, add the follwoing wiring to your display to set up an I2C bus between the screen and mbed. Capacative touch data is sent over I2C. In addition to the wiring described in the schematic above this, add the following:



Once the touchscreen is wired up, the humidity sensor can also be connected to the mbed. Use the following wiring diagram.





