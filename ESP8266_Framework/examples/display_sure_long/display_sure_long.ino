#include <WifiTool.h>
#include <defs.h>
#include <drivers.h>
#include <WifiScanNetwork.h>
#include <WiFiMem.h>
#include <Scheduler.h>
#include <Service_NTP.h>
#include <LM75.h>
#include <WiFiComunication.h>
#include <cstring>

#include <WifiTool.h>

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif
#include "myfont.h"


#include "ht1632.h"

#ifdef __AVR__
 #include <avr/io.h>
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#else
 #define PROGMEM
#endif

#include <Wire.h>

#define ST_DATA   10
#define TIME_DATA 11

WiFiTool *tool;
ESP8266WebServer *myserver;
HT1632* myHT;

void setup(void) {
  char deviceIp[32];   
  myHT = new HT1632(); 
  myHT->setBrightness(10);
  myHT->displayScrollingLine("ClockNet v0.1 - DLF", 0);
  myHT->displayEffect("Conn..",0);
  tool = new WiFiTool();
  myserver = tool->server;
  sprintf(deviceIp,"Ip - %s",(char*)((String)(tool->GetIp())).c_str());
  myHT->setBrightness(10);
  myHT->displayScrollingLine(deviceIp, 0);
    
}

int Sec_ = 0;
int Min_ = 0;
int Hou_ = 0;
char f =':'; 
void loop() {
  int tSec_ = tool->GetLTZSeconds();
  int tMin_ = tool->GetLTZMinutes();
  int tHou_ = tool->GetLTZHours();
  
    
  if ((tSec_ != Sec_) ||
      (tMin_ != Min_) ||
      (tHou_ != Hou_))
   {
      char Time[32];
      if (f == ':')
      {
        f = ' ';  
      }
      else
      {
        f = ':';
      }
      sprintf(Time,"%02d%c%02d",tHou_,f,tMin_);
      //myHT->fadeDown();            
      if(tMin_ != Min_){
        myHT->fadeDown();            
        int data[2];
        tool->GetDriversData(DTH11_SENSOR,(void*)&data);
     
        char temp[32];
        sprintf(temp,"Umid:%d%",data[0]);             
        myHT->setBrightness(10);
        myHT->displayScrollingLine(temp, 0);                 
        sprintf(temp,"Temp:%dC",data[1]); 
        myHT->displayScrollingLine(temp, 0);                 
        
        myHT->setBrightness(0);
        myHT->fadeUp();             
        myHT->setBrightness(10);         
        myHT->displayClock(Time,0);      
      }
      else{
        
        myHT->displayClock(Time,0);
      }
      Sec_ = tool->GetLTZSeconds();
      Min_ = tool->GetLTZMinutes();
      Hou_ = tool->GetLTZHours();
      
   }    
  tool->HandlServerEvent(); 
}

