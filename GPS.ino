/*
code belongs to this video: https://www.youtube.com/watch?v=dy2iygCZTIM
write by Moz for Youtube changel LogMaker360 26-10-2016
*/

 // I have a Data Logger Module Shield V1.0 for Arduino UNO SD Card
   // and a GY-NEO6MV2 new NEO-6M GPS Module NEO6MV2 gps
   //real time clock is included on the Data logger shield.
   
   
    #include <SoftwareSerial.h>
    #include <TinyGPS.h>
    
    #include <Wire.h>
	#include <SPI.h>
	#include <SD.h>
	#include <RTClib.h>
	#include <OneWire.h>
	
File dataFile;
DateTime now;
RTC_DS1307 RTC;
  
const int chipSelect = 10; // pin for the SD card logger.

     
    long lat,lon; // create variable for latitude and longitude object
     
    SoftwareSerial gpsSerial(3, 2); // create gps sensor connection                                                              4 , 3 in the video!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!. 
    TinyGPS gps; // create gps object
     
    void setup(){
    
      Serial.begin(115200); // connect serial  //for some olther gps sensor try Serial.begin(9600); and gpsSerial.begin(4800);
      gpsSerial.begin(9600); // connect gps sensor
      
       Wire.begin(); // real time clock
  RTC.begin();  // also for the real time clock
//check or the Real Time Clock is on
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
//setup SD card
   Serial.print("Initializing SD card...");

  // see if the SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  //write down the date (year / month / day         prints only the start, so if the logger runs for sevenal days you only findt the start back at the begin.
    now = RTC.now();
    dataFile = SD.open("gpsLOG.txt", FILE_WRITE);
    dataFile.print("Start logging on: ");
    dataFile.print(now.year(),DEC);
    dataFile.print('/');
    dataFile.print(now.month(),DEC);
    dataFile.print('/');
    dataFile.print(now.day(),DEC);
    dataFile.println(" ");
    dataFile.println("Latitude              Longitude              Time");
    dataFile.close();

    }
     
    void loop(){
    	
    	now = RTC.now();
  
  

//log the time and gps coordinaten every 10 seconds
 
    	
      while(gpsSerial.available()){ // check for gps data
       if(gps.encode(gpsSerial.read())){ // encode gps data
        gps.get_position(&lat,&lon); // get latitude and longitude
        // display position
        Serial.print("Position: ");
        Serial.print("coordinaat ");Serial.print(lat/1000000); Serial.print(".");Serial.print(lat%1000000);Serial.print(" ");// print latitude to serialmonitor
        Serial.print(", ");Serial.print(lon/1000000); Serial.print(".");Serial.println(lon%1000000);// print longitude to serialmonitor
         dataFile = SD.open("gpsLOG.txt", FILE_WRITE);
         if (dataFile) {
    
    dataFile.print(lat/1000000); dataFile.print("."); dataFile.print(lat%1000000); dataFile.print(" ");// print latitude to the SD card
    dataFile.print("            ");
    dataFile.print(lon/1000000); dataFile.print("."); dataFile.print(lon%1000000);// print longitude to SD Card
    
   
    dataFile.print("              ");
    
    dataFile.print(now.hour(),DEC);
    dataFile.print(":");
    dataFile.print(now.minute(),DEC);
    
    dataFile.print(":");
    dataFile.println(now.second(),DEC);
   
    dataFile.close();
    // print to the serial port too:
    Serial.println("data stored");
  }
  ///Serial.println("minute past");
 // }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening gpslog.txt");
  }//}
  
  delay(10000);
    	
       
       }
      }
    }
