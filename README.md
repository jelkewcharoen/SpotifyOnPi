# Singing in the Shower Assistant: ECE4180 Final Project

Group Members: Anna Carow and Jel Kewcharoen

## Concept

Singing in the Shower is a product that allows users to control the music playing on their Spotify account through a bluetooth speaker using a waterproof touchscreen. When you turn the shower on, the humidity and temperature sensor in the device will cause the screen to light up and prompt you if you want to begin playing music. From the touchscreen, you will have the option to start, pause, skip, change the volume, and change the playlist. The touchscreen will send signals to the Rasberry Pi, which is connected to both spotify and a bluetooth speaker. 

## Parts List

2.8" Adafruit TFT LCD with Touchscreen with ILI9341 Controller and capacative touch sensing

<img src="https://cdn-shop.adafruit.com/970x728/1770-00.jpg" width="200" height="150">

DHT11 Humidity Sensor

<img src="https://components101.com/sites/default/files/components/DHT11-Sensor.jpg" width="150" height="150">

Rasberry Pi 3
ARM mBed Microcontroller
Bluetooth Speaker

## Software

* mbed DmTFT Libaray to support the ILI9341 Screen Controller
* mbed FT6026 Libaray to support the FT6026 capacative touchscreen controller 
* mbed DHT Libaray to support the DHT11 Humidity Sensor
* Spotify API/GPIO Libaray
* RPC Stuff?


## Building the System

### mbed

To begin seeting up the mbed side of the system, import the DmTFT Library (https://os.mbed.com/users/displaymodule/code/DmTftLibrary/) into your project. This library gives the you availity to interface with a variety of different TFT LCD Libraries. For the screen used in this project, be sure the include the  DmTftIli9341.h header file. This library provides users with a variety of commands to draw on and print to the screen. Note that this disply has an SPI mode and an 8-bit mode to write to the screen. The library and provided pinout is for SPI mode.


| mbed | TFT LCD |
| --- | --- |
| GND | GND |
| Vout | Vin |
| NC  | 3Vo |
| p7  | CLK |
| p6  | MISO |
| p5  | MOSI |
| p8  | CS |
| p10  | D/C |
| p9 | RST |
| through 100 ohm resistor to Vout | LITE |
| Vout | IM3 |
| Vout | IM2 |
| Vout | IM1 |
| GND | IM0 |
| NC | CCS |
| NC | CD |


A second library, FT6026, is needed to make use the capacative touch capabilites of the LCD screen. Import the mbed FT6026 library (https://os.mbed.com/users/JackB/code/FT6206/file/35e21af6733c/FT6206.cpp/). After than is complete, add the follwoing wiring to your display to set up an I2C bus between the screen and mbed. Capacative touch data is sent over I2C. In addition to the wiring described in the schematic above this, add the following:

| mbed | TFT LCD Touchscreen |
| --- | --- |
| p27  | SCL |
| p28  | SDA |
| p21  | IQR |
| GND  | GND |

Once the touchscreen is wired up, the humidity sensor can also be connected to the mbed. Import the DHT Library (https://os.mbed.com/users/Wimpie/code/DHT/) it interface witht the device and use the following wiring diagram.

| mbed | DHT11 Humdity Sensor |
| --- | --- |
| Vin  | Vdd |
| p20  | S |
| GND  | GND |

### Rasberry Pi

### Pi - mbed GPIO Interface

In order to allow the mbed to send GPIO signals to the Pi when the screen is pressed, the following connections must be made between the Pi and mbed GPIO pins

| Pi | mbed |
| --- | --- |
| p17 | p22 |
| p17 | p23 |
| p17 | p24 |
| p17 | p25 |
| p17 | p26 |
| p17 | p14 |

## Connecting to Spotify

## User Interface

When not in use, the LCD Touchscreen simply displays the name of our project - "Singing in the Shower." The temperature and humidity sensor is used to detect if the shower has turned on. At this point, the screen will switch to a display with 6 options to choose between: play, pause, skip, increase volume, decrease volume, and change playlist.

(Image of the options here)

When any of these options are selected, a signal is sent via GPIO to the Node Red program running on the Pi. Node red will then send the command to the connected Spotify account. The Pi will be connected to a bluetooth speaker which music will play through.

If the 'Select Playlist' Option is selected, Node Red will send a list of available playlists to the mbed. A new screen will pop up with all the playlist options to choose from. Once a playlist is selected, the screen will return to displaying the default options (Pause, Skip, etc.).


## Video Demo

![image](https://github.gatech.edu/storage/user/33435/files/980a4377-1601-4ae7-baac-2e8e34ec0829)

