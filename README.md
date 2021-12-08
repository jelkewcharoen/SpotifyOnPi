# Singing in the Shower Assistant: ECE4180 Final Project

Group Members: Anna Carow and Jel Kewcharoen

## Concept

Singing in the Shower is a product that allows users to control the music playing on their Spotify account through a bluetooth speaker using a waterproof touchscreen. When you turn the shower on, the humidity and temperature sensor in the device will cause the screen to light up and prompt you if you want to begin playing music. From the touchscreen, you will have the option to start, pause, skip, change the volume, and change the playlist. The touchscreen will send signals to the Rasberry Pi, which is connected to both spotify and a bluetooth speaker. 

## Parts List

* 2.8" Adafruit TFT LCD with Touchscreen with ILI9341 Controller and capacative touch sensing
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
