# Singing in the Shower Assistant: ECE4180 Final Project

Group Members: Anna Carow and Jel Kewcharoen

## Concept

Singing in the Shower Assistant is a product that allows users to control the music playing on their Spotify account through a Bluetooth speaker using a touchscreen LCD. When you turn the shower on, the humidity and temperature sensor in the device will cause the screen to light up and allow you begin controlling music. From the touchscreen, you have the option to start, pause, skip, change the volume, and change the playlist. The touchscreen sends signals to a Rasberry Pi which is connected to both Spotify server and a Bluetooth speaker.

## Parts List

2.8" Adafruit TFT LCD with Touchscreen with ILI9341 Controller and capacitive touch sensing

<img src="https://cdn-shop.adafruit.com/970x728/1770-00.jpg" width="200" height="150">

DHT11 Humidity Sensor

<img src="https://components101.com/sites/default/files/components/DHT11-Sensor.jpg" width="150" height="150">

Rasberry Pi 3<br/>
ARM Mbed microcontroller<br/>
Bluetooth Speaker

## Software

* mbed DmTFT Library to support the ILI9341 Screen Controller
* mbed FT6026 Library to support the FT6026 capacitive touchscreen controller 
* mbed DHT Library to support the DHT11 Humidity Sensor
* Node-red GPIO Library
* Node-red SerialPort Library
* Librespot Spotify Client

## System Diagram

Below is a high-level system diagram showing how all the hardware and software components of the product interact.

<img width="715" alt="Screen Shot 2021-12-13 at 11 08 04 AM" src="https://github.gatech.edu/storage/user/33435/files/27c6d8fe-4321-4ea0-9313-2429b25e660a">

## Building the System

### mbed

To begin setting up the mbed side of the system, import the DmTFT Library (https://os.mbed.com/users/displaymodule/code/DmTftLibrary/) into your project. This library gives the you ability to interface with a variety of different TFT LCD screens. For the screen used in this project, be sure the include the  DmTftIli9341.h header file. This library provides users with a variety of commands to draw on and print to the screen. Note that this display has an SPI mode and an 8-bit mode to write to the screen. The library and provided pinout are for SPI mode.


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


A second library, FT6026, is needed to make use of the capacitive touch capabilities of the LCD screen. Import the mbed FT6026 library (https://os.mbed.com/users/JackB/code/FT6206/file/35e21af6733c/FT6206.cpp/). After that is complete, add the following wiring to your display to set up an I2C bus between the screen and mbed. Capacitive touch data is sent over I2C. In addition to the wiring described in the schematic above this, add the following:

| mbed | TFT LCD Touchscreen |
| --- | --- |
| p27  | SCL |
| p28  | SDA |
| p21  | IQR |
| GND  | GND |

Once the touchscreen is wired up, the humidity sensor can also be connected to the mbed. Import the DHT Library (https://os.mbed.com/users/Wimpie/code/DHT/) so it can interface with the device and use the following wiring diagram:

| mbed | DHT11 Humdity Sensor |
| --- | --- |
| Vin  | Vdd |
| p20  | S |
| GND  | GND |

### Rasberry Pi

The software on Pi is controlled using Node-red. Node-red is set up to start at reboot.

### Node-red

We use a Node-red Spotify library to connect to the spotify server, as well as GPIO and Serial Port libraries to communicate with mbed. GPIO is used when Mbed needs to send an action signal received on the LCD screen to Pi, and Serial port is used when strings, like playlist names, need to be exchanged. Below is a screen capture of the Node-red flow. The json file for this flow is also included in this repository.

<img width="632" alt="Screen Shot 2021-12-13 at 1 08 22 AM" src="https://github.gatech.edu/storage/user/39303/files/df3279e2-4d6f-4ee6-ad82-516a821299ad">

### Pi - Mbed GPIO Interface

In order to allow the mbed to send GPIO signals to the Pi when the screen is pressed, the following connections must be made between the Pi and mbed GPIO pins. Each pin corresponds to different actions: play, pause, skip, decrease volume, increase volume, and select a playlist.

| Pi | mbed |
| --- | --- |
| p17 | p22 |
| p17 | p23 |
| p17 | p24 |
| p17 | p25 |
| p17 | p26 |
| p17 | p14 |

## Connecting to Spotify

Librespot Spotify Client Open Source library is used to create a Spotify client for a specified account on Pi. When selecting play for the first time, Pi will execute a command to create a Spotify client server on Pi. This allows us to play music solely on the Pi and not on a smartphone. 

Node-red Spotify library connects to the Spotify API and allows us to control the Spotify account. Before playing any music, the account playback will need to be transferred to the newly create client/device, which is the Raspberry Pi. Since Pi is automatically connected to the Bluetooth speaker that we previously set up, the music will go to the speaker right away. 

## User Interface

All the components are contained in a 3D printed container, with one open side for easy access, and two holes for the LCD screen and the sensors.

When not in use, the LCD Touchscreen simply displays the name of our project - "Singing in the Shower Assistant."  

<img src="https://github.gatech.edu/storage/user/39303/files/53a6287a-43f4-4171-883d-94454a20c97a" width="600">

A DHT11 temperature and humidity sensor is used to detect if the shower has turned on.

<img src="https://github.gatech.edu/storage/user/39303/files/001f22de-0f62-457c-9f8d-8d6743354081" width="600">

When the temperature and humidity reach the set critical values that indicate the shower is on, the screen will switch to a display with 6 options to choose between: play, pause, skip, increase volume, decrease volume, and change playlist. 

<img src="https://github.gatech.edu/storage/user/39303/files/7e6221cc-dd99-4cde-8563-77a002ed1462" width="600">

If the 'Playlist' option is selected, Node Red will send a list of available playlists to the mbed. A new screen will pop up with the playlist options to choose from. Once a playlist is selected, mbed sends the selected playlist name back to Pi, and the screen will return to displaying the default options (Pause, Skip, etc.).

<img src="https://github.gatech.edu/storage/user/39303/files/d0f0ec3d-1ecb-450f-a111-36ef19c5ef00" width="600">

## Video Demo

Demo Number One: https://youtu.be/mLpKBZplPH4 (Temperature/Humidity Sensor Activating the Screen)

Demo Number Two: https://youtu.be/xb4JrIlmrYs (Spotify Integration)
