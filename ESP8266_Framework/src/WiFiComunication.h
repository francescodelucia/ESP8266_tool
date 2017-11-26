#ifndef _WIFICOMUNICATION_
#define _WIFICOMUNICATION_

#include <defs.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <cstring>

extern "C" {
#include "user_interface.h"
}

class WiFiComunication{
	private:
		/* Set these to your desired credentials. */
		unsigned char SSID[32];
		unsigned char PWD[32];	
		char IPAdd[16];			
		IPAddress devIP;
		int isAP;
	
			
	protected:
		//char MAC_char[17];
		char deviceName[32];
		char devicePwd[32];
	public:
		char MAC_char[20];
		WiFiComunication()
		{ 
			//wifi_set_phy_mode(PHY_MODE_11g);
			this->isAP = ACCESS_POINT;	
			memset(this->IPAdd,0,16);			
		
#ifdef _WIFICOM_DEBUG_
			Serial.println(BASE_COSTRUCTOR);
			Serial.printf("%s\n",this->MAC_char);
#endif			
			//sprintf(deviceName,"ESP8266%s\0",this->MAC_char);
			//sprintf(devicePwd,"%s\0",this->MAC_char);
#ifdef _WIFICOM_DEBUG_								
			Serial.printf("MAC %s\n",this->MAC_char);					
			Serial.printf("NAME %s\n",this->deviceName);					
			Serial.printf("PWD %s\n",this->devicePwd);
#endif
		}
		WiFiComunication( char *ssid,char *pwd,char *deviceName,char *devicePwd) : WiFiComunication()
		{			
			memset(this->MAC_char,0,20);
			memset(this->deviceName,0,32);
			memset(this->devicePwd,0,32);
			memset(this->SSID,0,32);
			memset(this->PWD,0,32);
			GetMAC();
			memcpy((void*)this->SSID,(const void*)ssid,strlen((const char*)ssid));
			memcpy((void*)this->PWD,(const void*)pwd,strlen((const char*)pwd));	
			memcpy((void*)this->deviceName,(const void*)deviceName,strlen((const char*)deviceName));
			memcpy((void*)this->devicePwd,(const void*)devicePwd,strlen((const char*)devicePwd));	
			
#ifdef _WIFICOM_DEBUG_				
			Serial.println(OVERLOAD_COSTRUCTOR);
#endif
		}
		WiFiComunication( char *ssid,char *pwd) : WiFiComunication()
		{
			memset(this->MAC_char,0,20);
			memset(this->deviceName,0,32);
			memset(this->devicePwd,0,32);
			memset(this->SSID,0,32);
			memset(this->PWD,0,32);		

			memcpy((void*)this->SSID,(const void*)ssid,strlen((const char*)ssid));
			memcpy((void*)this->PWD,(const void*)pwd,strlen((const char*)pwd));	
			GetMAC();
			sprintf(this->deviceName,"ESP8266%s\0",this->MAC_char);
			sprintf(this->devicePwd,"%s\0",this->MAC_char);

		}
		int CreateAP()
		{	
#ifdef _WIFICOM_DEBUG_				
			Serial.println(AP_SERVER_CREATE);
#endif
			if((strlen((const char*)this->SSID)>0) && strlen((const char*)this->SSID)>0)
			{
				WiFi.softAP((const char*)this->SSID,(const char*)this->PWD);
			}
			else
			{
				//sprintf(deviceName,"ESP8266%s",this->MAC_char);
				//sprintf(devicePwd,"%s",this->MAC_char);
				WiFi.softAP(this->deviceName,this->devicePwd);			
			}
			this->isAP = ACCESS_POINT;
			this->devIP  = WiFi.softAPIP();
			return ACCESS_POINT;
		}
		int  CreateWFClient()
		{
			
#ifdef _WIFICOM_DEBUG_				
			Serial.println(AP_CLIENT_CREATE);				
#endif
			int k=0;
			if((strlen((const char*)this->SSID)>0) && strlen((const char*)this->PWD)>0)
			{
				WiFi.begin((const char*)this->SSID,(const char*)this->PWD);
			}
			else
			{				
#ifdef _WIFICOM_DEBUG_				
				Serial.println(NOT_CONNECTED);													
#endif				
				//sprintf(this->deviceName,"ESP8266%s",this->MAC_char);
				//sprintf(this->devicePwd,"%s",this->MAC_char);
				WiFi.softAP(this->deviceName,this->devicePwd);	
				this->isAP = ACCESS_POINT; 
				this->devIP = WiFi.softAPIP(); 
				delay(100);
#ifdef _WIFICOM_DEBUG_				
				Serial.println("IP");
				Serial.println(this->devIP);
#endif	
				return ACCESS_POINT;
			}
						
			while (WiFi.status() != WL_CONNECTED) {				
#ifdef _WIFICOM_DEBUG_				
				Serial.print(".");
#endif
				delay(1000);				
				k ++;
				if(k==20){				
#ifdef _WIFICOM_DEBUG_				
					Serial.println(NOT_CONNECTED);															
#endif				
					//sprintf(this->deviceName,"ESP8266%s",this->MAC_char);
					//sprintf(this->devicePwd,"%s",this->MAC_char);
					WiFi.softAP(this->deviceName,this->devicePwd);
					this->isAP = ACCESS_POINT; 
					this->devIP = WiFi.softAPIP(); 
#ifdef _WIFICOM_DEBUG_				
					Serial.println("IP");
					Serial.println(this->devIP);
#endif	
					return ACCESS_POINT;
				}				
			}
			this->isAP = WIFI_CLIENT;
			delay(500);
			this->devIP = WiFi.localIP();
#ifdef _WIFICOM_DEBUG_				
			Serial.println("IP");
			Serial.println(this->devIP);
#endif	
			return WIFI_CLIENT;			
		}
		char* GetMAC()
		{				
			
#ifdef _WIFICOM_DEBUG_				
			Serial.println("1");													
#endif		
			uint8_t MAC_array[6];
			
#ifdef _WIFICOM_DEBUG_				
			Serial.println("2");													
#endif		
			WiFi.macAddress(MAC_array);
#ifdef _WIFICOM_DEBUG_				
			Serial.println("3");													
#endif					
			for (int i = 0; i < sizeof(MAC_array); ++i){
#ifdef _WIFICOM_DEBUG_				
				Serial.print("4-");										
				Serial.println(i);										
#endif		
				sprintf(this->MAC_char,"%s%02x",this->MAC_char,MAC_array[i]);
			}
#ifdef _WIFICOM_DEBUG_				
			Serial.println("5");															
#endif		
			return this->MAC_char;
		}
		String GetIp()
		{		
			return 	this->devIP.toString();
		}
		int IsAP(){return this->isAP;}
		
};

#endif
