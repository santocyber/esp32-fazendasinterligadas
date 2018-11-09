# esp32storming
Rastreamento usando ESP32 e Bluetooth Low Energy BLE
====================================================
- Material Necessario
-- Modulo GPS neo6mv2

https://www.aliexpress.com/item/2pcs-lot-GY-NEO6MV2-new-GPS-module-with-Flight-Control-Flight-Control-EEPROM-MWC-APM2-5/1811853522.html?spm=2114.search0104.3.2.3f0b3941mAC4IY&ws_ab_test=searchweb0_0,searchweb201602_5_10065_10068_10547_319_5730915_317_10548_10696_5728811_453_10084_454_10083_10618_10307_537_536_5729815_5733215_5733315_328_10059_10884_5731015_5733115_10887_100031_5733415_321_5730015_322_10103_5733515_5733615-5733215,searchweb201603_2,ppcSwitch_0&algo_expid=bf4bd8cb-71c6-4e7d-b8ba-0d2d76123687-0&algo_pvid=bf4bd8cb-71c6-4e7d-b8ba-0d2d76123687

-- ESP32 com bateria 18650

https://www.aliexpress.com/item/ESP32-ESP-32S-For-WeMos-WiFi-Wireless-Bluetooth-Development-Board-CP2102-CP2104-Module-With-18650-lithium/32840831496.html?spm=2114.search0104.3.88.5cfc3fbbwQ5FEL&ws_ab_test=searchweb0_0,searchweb201602_5_10065_10068_10547_319_5730915_317_10548_10696_5728811_453_10084_454_10083_10618_10307_537_536_5729815_5733215_5733315_328_10059_10884_5731015_5733115_10887_100031_5733415_321_5730015_322_10103_5733515_5733615,searchweb201603_2,ppcSwitch_0&algo_expid=417e3835-0cf5-4207-a942-0f5a996f2a55-12&algo_pvid=417e3835-0cf5-4207-a942-0f5a996f2a55

- GPS Vizualizer
http://www.gpsvisualizer.com/


Installation instructions for Debian / Ubuntu OS
=================================================

- Install latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
- Open Terminal and execute the following command (copy->paste and hit enter):

  ```bash
  sudo usermod -a -G dialout $USER && \
  sudo apt-get install git && \
  wget https://bootstrap.pypa.io/get-pip.py && \
  sudo python get-pip.py && \
  sudo pip install pyserial && \
  mkdir -p ~/Arduino/hardware/espressif && \
  cd ~/Arduino/hardware/espressif && \
  git clone https://github.com/espressif/arduino-esp32.git esp32 && \
  cd esp32 && \
  git submodule update --init --recursive && \
  cd tools && \
  python2 get.py
  ```
- Restart Arduino IDE

Iniciando o display sample code
=================================================
#include <Wire.h>
#include "SSD1306.h" 
 
SSD1306  display(0x3c, 21, 22);
 
void setup() {
 
  display.init();
  display.drawString(0, 0, "Hello World");
  display.display();
}
 
void loop() {}


Analog Sound Sample Code
=================================================

void setup() 
{ Serial.begin(9600); 
// open serial port, set the baud rate to 9600 bps 
} void loop()
{ int val; val=analogRead(13); 
//connect mic sensor to Analog 0 
Serial.println(val,DEC);
//print the sound value to serial
delay(100); }


Qrcode Sample Code
=================================================
#include <Wire.h>
#include "SSD1306.h"
#include <qrcode.h>
 
SSD1306 display(0x3c, 21, 22);
QRcode qrcode (&display);
 
void setup() {
 
  display.init();
  display.display();
 
  qrcode.init();
  qrcode.create("Hello from esp32");
}
 
void loop() {}



Clap Switch Sample Code
=================================================
//---------------------------------------------------------------------
//  Program:      clap switch
//
//  Description:  Switches on an LED when hands are clapped.
//                Electret microphone connected to A2, LED and series
//                resistor connected to digital pin 2
//
//  Date:         6 April 2016      Author: W.A. Smith
//                http://startingelectronics.org
//---------------------------------------------------------------------
void setup() {
  Serial.begin(9600);             // using serial port to check analog value
  pinMode(2, OUTPUT);             // LED on digital pin 2
}

void loop() {
  int analog_val;                 // analog value read from A2
  static bool led_state = false;  // current state of LED
  
  analog_val = analogRead(A2);

  if (analog_val > 10) {          // trigger threshold
    // toggle LED
    if (led_state) {
      led_state = false;          // LED was on, now off
      digitalWrite(2, LOW);
      Serial.println(analog_val); // print analog value for debug purposes
    }
    else {
      led_state = true;
      digitalWrite(2, HIGH);      // LED was off, now on
      Serial.println(analog_val);
    }
    delay(50);  // wait for clap noise to subside
  }
}
