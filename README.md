# esp32storming
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


Mqtt conect
=================================================

#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "InternetSA";
const char* password = "semsenha";
const char* mqttServer = "iot.mirako.org";
const int mqttPort = 1883;
const char* mqttUser = "mqtt";
const char* mqttPassword = "mqtt";
int LED_BUILTIN = 2;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    pinMode (LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting to WiFi:");
        Serial.println(ssid);
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.println("Connecting to MQTT…");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
            Serial.println("connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    Serial.print("Tentando enviar a mensagem");
    client.publish("oi", "Hello from ESP32");
    client.subscribe("oi");

}

void callback(char* topic, byte* payload, unsigned int length) {

    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }


    Serial.println();
}

void loop() {
    client.loop();
}
