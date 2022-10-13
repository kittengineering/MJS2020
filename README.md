# MJS2020
MJS2020 platform board repository.

## MJS 2020 ##

Current How To to get code on the board:
* Note: under windows Arduino IDE sometimes doesn't find the port. Yet flashing the board still works:  set:  Tools -> serial -> refers to SerialUSB, and tools -> USB Type -> Serial, and tools -> programmer -> STM32 BOOTLOADER. Click "upload" and it should start uploading the software.
* https://github.com/kittengineering/ArduinoCore-stm32l0
* Board configuration for arduino IDE: https://github.com/meetjestad/mjs_boards/raw/master/package_meetjestad.net_index.json

## Resources ##

On https://github.com/meetjestad has a lot of information about these board. Note that depending on the firmware you might have a V1, V2, V3 or V4. Thankfully you can ask the board model using Arduino IDE

https://github.com/obokhove/wetropolis20162020 is another project that uses this board.

### Board overview ###
![alt text](https://github.com/kittengineering/MJS2020/blob/main/MJS2020.jpg?raw=true)

#### Example of a board with a mounted OLED screen and temperature and humidity sensor ###

![alt text](https://github.com/kittengineering/MJS2020/blob/main/MJS2020_WithAirSensorAndOLED.jpg?raw=true)


## Contact information ##
If you want to contact us for information about this board please send an email to mjs2020(at)kitt.nl.
