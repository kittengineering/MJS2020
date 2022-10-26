# MJS2020
MJS2020 platform board repository.

## MJS 2020 ##

## Getting started ##
* Note if your board is fresh off the shelf and doesn't have any firmware installed, follow the "Installing firmware -> OS Specific Setup" as descibed further down.

### Install Arduino IDE ###
 1. In arduino IDE go to File -> Preferences and in "Additional boards manager URLS" add https://github.com/meetjestad/mjs_boards/raw/master/package_meetjestad.net_index.json and press ok
 2. go to Tools -> Board -> Board manager, type in "STM32", the "Meet je stad! STM32LO Boards" should appear. "Boards included in this package" states(among other things) "MJS2020-PROTO4". Press install.
 3. go to tools -> programmer and select STM32 BOOTLOADER (STM32LO)

### Getting example sketches. ###
 1. either clone or download from this git repository to get a .ino sketch directly in the software subfolders.
 2. To install a sketch set the board into "flash mode". 
 3. To set the board in flash mode: power on the board (using either USB or battery power ). set the dip switch in the middle of the board on "On", hold down the "boot" button and toggle (press shortly) the "reset" button when the board is powered.
 4. Note: under windows Arduino IDE sometimes doesn't find the port. Yet flashing the board still works:  set:  Tools -> serial -> refers to SerialUSB, and tools -> USB Type -> Serial, and tools -> programmer -> STM32 BOOTLOADER. Click "upload" and it should start uploading the software.
 5. if it shows [Downloading ==== ] and then "Download succeded" but follows with an error message, the .ino sketch should still work.
 
 
### Connecting the board ###
 1. follow https://github.com/meetjestad/mjs_bootstrap 

### Installing firmware ###
* This only needs to be done once: when the board comes off the shelf from the factory.
* Your board already has firmware installed if, for example, it displays text on the LCD, sends messages over serial ports, or "does anything besides just sitting there".

#### OS Specific Setup

##### Linux

 1. Go to ~/.arduino15/packages/kittengineering/hardware/stm32l0/```<VERSION>```/drivers/linux/
 2. sudo cp *.rules /etc/udev/rules.d
 3. reboot

#####  Windows

###### STM32 BOOTLOADER driver setup for MJS2020 boards

 1. Download [Zadig](http://zadig.akeo.ie)
 2. Plugin STM32L0 board and toggle the RESET button while holding down the BOOT button
 3. Let Windows finish searching for drivers
 4. Start ```Zadig```
 5. Select ```Options -> List All Devices```
 6. Select ```STM32 BOOTLOADER``` from the device dropdown
 7. Select ```WinUSB (v6.1.7600.16385)``` as new driver
 8. Click ```Replace Driver```

###### USB Serial driver setup for MJS2020 boards (Window XP / Windows 7 only)

 1. Go to ~/AppData/Local/Arduino15/packages/kittengineering/hardware/stm32l0/```<VERSION>```/drivers/windows
 2. Right-click on ```dpinst_x86.exe``` (32 bit Windows) or ```dpinst_amd64.exe``` (64 bit Windows) and select ```Run as administrator```
 3. Click on ```Install this driver software anyway``` at the ```Windows Security``` popup as the driver is unsigned


## Resources ##

On https://github.com/meetjestad has a lot of information about these board. Note that depending on the firmware you might have a V1, V2, V3 or V4. Thankfully you can ask the board model using Arduino IDE

https://github.com/obokhove/wetropolis20162020 is another project that uses this board.

### Board overview ###
![alt text](https://github.com/kittengineering/MJS2020/blob/main/MJS2020.jpg?raw=true)

#### Example of a board with a mounted OLED screen and temperature and humidity sensor ###

![alt text](https://github.com/kittengineering/MJS2020/blob/main/MJS2020_WithAirSensorAndOLED.jpg?raw=true)


## Contact information ##
If you want to contact us for information about this board please send an email to mjs2020(at)kitt.nl.
