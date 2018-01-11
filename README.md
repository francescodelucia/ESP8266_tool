# ESP8266_tool
Framework for simple develop Wifi ESP8266 application with Arduino IDE.

[API in WikiPage](https://github.com/francescodelucia/ESP8266_tool/wiki)

This FramewWork is composed by varius Classes (C++) and can help programmer 
to develope complex application in tiny time. Try to semplify the creation 
of complex Project with a lot peripherals. Help to use scheduling Routine,
event manager.

The Framework contains structural classes for:

- Manage Network comunication
- Manage Timing event (like a tiny sheduler)
- Manage Sensor on I2C  protocol for read Values , drivers avalibe now :
   *  [HT1632 (display)](https://github.com/francescodelucia/ESP8266_tool/blob/master/ESP8266_Framework/examples/Meteo_Temp_clock/Meteo_Temp_clock.ino)
   * [DHT11 (Temp Humidity Sensor)](https://github.com/francescodelucia/ESP8266_tool/blob/master/ESP8266_Framework/examples/Meteo_Temp_clock/Meteo_Temp_clock.ino)
   * LM75 (I2C Temp Sensor)
   * [SSD1306](https://github.com/francescodelucia/ESP8266_tool/blob/master/ESP8266_Framework/examples/SSD1306/SSD1306.ino) (I2C Display - Thanks to https://acrobotic.com/)
   * Other driver soon.
- Manage a simplify a creation of [Telegram Bot](https://github.com/francescodelucia/ESP8266_tool/blob/master/ESP8266_Framework/examples/TestWithBot/TestWithBot.ino) 
- Manage and Creation of Web Pages for personal Applications.
- NTP Client for the timing sincronization.
- [Meteo Information from Web](https://github.com/francescodelucia/ESP8266_tool/blob/master/ESP8266_Framework/examples/Meteo_Temp_clock/Meteo_Temp_clock.ino) cutomizable (Remeber to subribe to [OpenWeather](https://openweathermap.org/) and obtain API_KEY for maching request an add API_KEY on [meteo.h](https://github.com/francescodelucia/ESP8266_tool/blob/master/ESP8266_Framework/src/meteo.h) file).


Base Wifi Settings
when u start first time the device.You find Ap with name like this ESP8266<mac_device> the password for device is
<mac_device>:
  es. AP-SSID -> ESP82665ccf7f10b141
      AP-PASSWORD -> 5ccf7f10b141

Naturally you can change AP-SSID and Password on manager page.

Base Home page OutOfBox give information of Device
http://x.x.x.x

Setting page can find at
http://x.x.x.x/manager

In the manager page you can:

- Change Ap to connect like client
- Enable NTP ad decide the server
- Change Local Time
- Update Firmware OTA
- Reboot device

Basic use of WiFiTool Framework.

	#include <WifiTool.h>


	WiFiTool *tool;

	void setup(void)
	{ 
      //This Enable all funtionality of Wifi Manager, Memory Store ( Wifi,NTP,Time Zone), Remote Update.
	    tool = new WiFiTool(); 
	}

	void loop(void)
	{
	  tool->HandlServerEvent(); //This is only routine you need call on loop.
	}

How you can see on simple program, you have immediatlly, a full Wifi Manger, Event Manager and other.

Use of WiFiTool Framework, with integrated event.

	#include <WifiTool.h>

	WiFiTool *tool;

	/*
	Routine for Show Time Clock
	*/
	void Orologio(){
		/*
		This data are syncronized on Time Server with internal NTP Client ( you
		can Manage on Web Manager Page )
		*/
		int tSec_ = tool->GetLTZSeconds();
		int tMin_ = tool->GetLTZMinutes();
		int tHou_ = tool->GetLTZHours();
		char Time[32];
		sprintf(Time,"%02d:%02d",tHou_,tMin_);
		Serial.printf("Time:%s\n",&Time[0]); 
	}


	void setup(void) {
		char deviceIp[32];
		tool = new WiFiTool(); 
		sprintf(deviceIp,"Ip - %s",(char*)((String)(tool->GetIp())).c_str());
		/*
			All routine external are added to internal Framework Scheduler
			This routine (Orologio) are called every 1000ms (1sec) 
		*/
		tool->AddEvent((void*)Orologio,1000);
	}

	void loop() {
		tool->HandlServerEvent(); 
	}
	
Use of WiFiTool Framework, with DTH11 driver.

	/*
	  with this declaration you enable  condictional compilation to use 
	  the driver DTH11 Humidity/Tempearture sensor	  
	  
	  Note: for the referiment of the other drivers avalible see source code.
	  Everyone are free to add other.
	*/
	#define DTH11_TEMP_SENSOR ENABLE
	#include <WifiTool.h>

	WiFiTool *tool;

	/* 
	 Routie for Read data from Sensor DHTxx family 
	*/
	void GetLocalTemp(){
	  char temp[32];	  
	  int data[2];
	  //This routine fill pointer 'data' with temperature and humidity read from DTH11 sensor
	  tool->GetDriversData(DTH11_SENSOR,(void*)&data);
	  sprintf(temp,"Temp %dC Humid %d%",data[1],data[0]);
	  Serial.printf("%s\n",&temp[0]);                                    
	}

	void setup(void) {
	  char deviceIp[32];   
	  tool = new WiFiTool();
	  /*
	   All routine external are added to internal Framework Scheduler 
	  */
	  tool->AddEvent((void*)GetLocalTemp,10000);
	}

	void loop() {
	  tool->HandlServerEvent();  
	}
