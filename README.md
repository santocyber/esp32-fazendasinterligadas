# esp32Fazendas Interligadas
Instalando ESP32 
==================

Installation instructions for Debian / Ubuntu OS
--------------------------------

- Install latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
- Open Terminal and execute the following command (copy->paste and hit enter):

  ```bash
  sudo apt-get install git 
  
  sudo usermod -a -G dialout $USER && \
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


Analog Sound Sample Code
=================================================



Qrcode Sample Code
=================================================


Clap Switch Sample Code
=================================================


Mqtt conect
=================================================

iot.mirako.org:1883
