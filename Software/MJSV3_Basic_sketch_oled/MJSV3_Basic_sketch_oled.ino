/*******************************************************************************
   Copyright (c) 2016 Thomas Telkamp, Matthijs Kooijman, Bas Peschier, Harmen Zijp

   Permission is hereby granted, free of charge, to anyone
   obtaining a copy of this document and accompanying files,
   to do whatever they want with them without any restriction,
   including, but not limited to, copying, modification and redistribution.
   NO WARRANTY OF ANY KIND IS PROVIDED.

   In order to compile the following libraries need to be installed:
   - SparkFunHTU21D: https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library
   - NeoGPS (mjs-specific fork): https://github.com/meetjestad/NeoGPS
   - Adafruit_SleepyDog: https://github.com/adafruit/Adafruit_SleepyDog
   - lmic (mjs-specific fork): https://github.com/meetjestad/arduino-lmic
 *******************************************************************************/

// include external libraries

#include <SPI.h>
#include <Wire.h>
#include "STM32L0.h"
#define DEBUG true
#define OLED true

#ifdef OLED
#include <Arduino.h>
#include <U8g2lib.h>
#endif

#ifdef OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

// Baudrate for hardware serial port
const uint16_t SERIAL_BAUD = 9600;

float const BATTERY_DIVIDER_RATIO = (1.0 + 1.0) / 1.0;
uint8_t const BATTERY_DIVIDER_PIN = PIN_BATTERY;
auto const BATTERY_DIVIDER_REF = AR_DEFAULT;
uint16_t const BATTERY_DIVIDER_REF_MV = 3000;

float const SOLAR_DIVIDER_RATIO = (2.0 + 1.0) / 1.0;
uint8_t const SOLAR_DIVIDER_PIN = PIN_SOLAR;
auto const SOLAR_DIVIDER_REF = AR_DEFAULT;
uint16_t const SOLAR_DIVIDER_REF_MV = 3000;

#define GPS_SERIAL Serial2
uint8_t const GPS_ENABLE_PIN = PIN_ENABLE_3V_GPS;

// define various pins
uint8_t const LED_PIN = LED_BUILTIN;
uint8_t const LED_ON = LOW;



#if defined(SERIAL_IS_SERIALUSB) || defined(SERIAL_IS_CONFIGURABLE)
bool wait_for_usb_configured(unsigned long timeout) {
    unsigned long start = millis();
    while (!USBDevice.connected() && millis() - start < timeout) /* wait */;

    return USBDevice.connected();
}

bool wait_for_serialusb_opened(unsigned long timeout) {
    unsigned long start = millis();
    while (!SerialUSB && millis() - start < timeout) /* wait */;

    return (bool)SerialUSB;
}

bool setup_serialusb() {
  // First, see if we are configured at the USB device level. This
  // happens automatically when USB is plugged into an USB host without
  // any user interaction, typically within a couple of hundred ms. If
  // that doesn't happen quickly, assume we're not connected to USB at
  // all and fail.
  if (wait_for_usb_configured(1000)) {
    // USB device was configured, so start serial port.
    SerialUSB.begin(0);

    // Then, see if the serial port is actually opened. Because this
    // needs action from the user, give them ample time for this.
    if (wait_for_serialusb_opened(10000)) {
      // Serial port was opened, we can use it.
      //
      // Wait a bit more, otherwise some clients (e.g. minicom on Linux) seem to miss the first bit of output...
      delay(250);

      return true;
    } else {
      // Stop the serial port again so any prints to it are ignored,
      // rather than filling up the buffer and eventually locking up
      // when the buffer is full.
      SerialUSB.end();
    }
  }

  return false;
}
#endif // defined(SERIAL_IS_SERIALUSB) || defined(SERIAL_IS_CONFIGURABLE)

bool setup_serial() {
  #if defined(SERIAL_IS_SERIALUSB)
  // For SerialUSB, things are more tricky, since the computer can only
  // open the serial port *after* the board started. This delays
  // startup to give the computer a chance to open the port so it can
  // see all messages printed to serial.
  setup_serialusb();
  #elif defined(SERIAL_IS_CONFIGURABLE)
  if (setup_serialusb()) {
    // SerialUSB port was openend, use that
    ConfigurableSerial = &SerialUSB;
  } else {
    // SerialUSB port was not opened, use hardware serial port instead
    Serial1.begin(SERIAL_BAUD);
    ConfigurableSerial = &Serial1;
  }
  #else
  #error "Unknown serial setup"
  #endif
}

void setup() {
  writeLed(0xff0000); // red
  delay (500);
  writeLed(0x00ff00); // green
  delay (500);
  writeLed(0x0000ff); // blue
  delay (500); 
  writeLed(0xff0000); // red
  delay (5000); 
  // when in debugging mode start serial connection
  if(DEBUG) {
    setup_serial();
    Serial.println(F("Starting Setup"));

  if (OLED) {
    u8g2.begin();  
  }
  }

  // setup switched ground and power down connected peripherals (GPS module)
  pinMode(GPS_ENABLE_PIN , OUTPUT);
  digitalWrite(GPS_ENABLE_PIN, LOW);
  // Switch off 5V supply
  pinMode(PIN_ENABLE_5V, OUTPUT);
  digitalWrite(PIN_ENABLE_5V, LOW);
  // Switch off 3V sensor supply
  pinMode(PIN_ENABLE_3V_SENS, OUTPUT);
  digitalWrite(PIN_ENABLE_3V_SENS, LOW);
}

void loop() {
  // We need to calculate how long we should sleep, so we need to know how long we were awake
  unsigned long startMillis = millis();
  Serial.println(F("Verbose"));
  if (OLED) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  
  delay (1000);
}


uint16_t readVcc()
{
  #ifdef __AVR__
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  // Wait a bit before measuring to stabilize the reference (or
  // something).  The datasheet suggests that the first reading after
  // changing the reference is inaccurate, but just doing a dummy read
  // still gives unstable values, but this delay helps. For some reason
  // analogRead (which can also change the reference) does not need
  // this.
  delay(2);

  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  uint16_t result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
  #elif defined(ARDUINO_ARCH_STM32L0)
  return STM32L0.getVDDA() * 1000;
  #endif
}



void writeSingleLed(uint8_t pin, uint8_t val) {
  // Work around a problem that analogWrite(255) is still
  // low for 1/256th of the time. Use HIGHZ for zero to save a
  // little power by disabling the digital pin driver completely. Also,
  // work around that analogWrite(4095) is not fully high (which is
  // problematic on PROTO2, because it means the led does not fully turn
  // off).
  // https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0/issues/104
  if (val == 0) {
    pinMode(pin, HIGHZ);
  } else {
    // Use the full 12-bits resolution (so analogWrite accepts 0-4095).
    // By not scaling val up, this effectively dims the led 4x (but
    // without rounding, so you can still produce tiny color
    // differences).
    analogWriteResolution(10);
    #if defined(ARDUINO_MJS2020_PROTO2)
    // On PROTO2, the LED is active-low
    analogWrite(pin, 1023 - val);
    #else
    analogWrite(pin, val);
    #endif
    // Reset back to the default, which other code probably expects.
    analogWriteResolution(8);
  }
 }

void writeLed(uint32_t rgb) {
  writeSingleLed(PIN_LED_RED, (rgb >> 16));
  writeSingleLed(PIN_LED_GREEN, (rgb >> 8));;
  writeSingleLed(PIN_LED_BLUE, (rgb >> 0));
  }
