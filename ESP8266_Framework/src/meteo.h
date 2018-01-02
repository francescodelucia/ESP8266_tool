#ifndef METEO_H
#define METEO_H

#ifndef METEO_WEB_DATA
	#define METEO_WEB_DATA DISABLE
#endif
#if METEO_WEB_DATA == ENABLE
	#define METEO_WEB 0x02
	#include <ArduinoJson.h>
	#include <cstring>
	#include <ESP8266HTTPClient.h>
	#ifndef APPID
		#define APPID "YOUR-OPENWEATHER-API-TOKEN"
	#endif
	#ifndef LOC_
		#define LOC_ "Rome"
	#endif
	#define OPEN_WEATHER_API(loc,appid) "http://api.openweathermap.org/data/2.5/weather?q=" loc ",it&units=metric&appid=" appid 
	#define OPEN_WEATHER_URL OPEN_WEATHER_API(LOC_,APPID)
	struct meteo_data{
		char temp[6];
		char humid[6];
		char temp_min[6];
		char temp_max[6];
		char weath[20];
		char description[20];
		char weath_icon[6];
		char localita[20];
	};
	



class Meteo {
	  private:
		byte data_meteo[90];
	  public:
	    Meteo()
	    {
			memset(&this->data_meteo[0],0,sizeof(struct meteo_data));
		};
		void read(){
			HTTPClient http;  //Declare an object of class HTTPClient			
			http.begin(OPEN_WEATHER_URL);  //Specify request destination			
			int httpCode = http.GET();//Send the request 
			if (httpCode > 0) { //Check the returning code
				String payload = http.getString();//Get the request response payload
				Serial.println(payload);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& root = jsonBuffer.parseObject(payload);
				sprintf((char*)&this->data_meteo[0],root["main"]["temp"].as<char*>());
				sprintf((char*)&this->data_meteo[6],root["main"]["humidity"].as<char*>());
				sprintf((char*)&this->data_meteo[12],root["main"]["temp_min"].as<char*>());
				sprintf((char*)&this->data_meteo[18],root["main"]["temp_max"].as<char*>());
				sprintf((char*)&this->data_meteo[24],root["weather"][0]["main"].as<char*>());
				sprintf((char*)&this->data_meteo[44],root["weather"][0]["description"].as<char*>());
				sprintf((char*)&this->data_meteo[64],root["weather"][0]["icon"].as<char*>());
				sprintf((char*)&this->data_meteo[70],root["name"].as<char*>());
			}
			http.end();//Close connection
		};
		
		void getData(void* data){
			memcpy(data,&this->data_meteo[0],sizeof(struct meteo_data));
		};
};
Meteo *meteo=new Meteo();
#endif //#if METEO_WEB_DATA == ENABLE
#endif

