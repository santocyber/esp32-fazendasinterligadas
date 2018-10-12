# esp32storming

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


Analogic Sound Sample Code
=================================================
void setup()
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}
void loop()
{
      int val;
      val=analogRead(0);   //connect mic sensor to Analog 0
      Serial.println(val,DEC);//print the sound value to serial        
      delay(100);
}


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
