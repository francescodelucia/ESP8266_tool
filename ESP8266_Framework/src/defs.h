#ifndef _DEFINITIONS_
#define  _DEFINITIONS_

#define ENABLE		1
#define DISABLE		0




#include <macros.h>
#include <devices/dht11.h>
#include <devices/ht1632.h>
#include <devices/LM75.h> 
#include <devices/SSD1306.h> 
#include <meteo.h>

#define _BAUD_ 115200

#define _WIFICOM_DEBUG_
#define _WIFIMEM_DEBUG_
#define _WIFI_DEBUG_
#define _NTP_DEBUG_

#define UDP_PORT 			2390 // local port to listen for UDP packets
#define NTP_SERVER_			"0.it.pool.ntp.org"
#define NTP_PACKET_SIZE 	48	// NTP time stamp is in the first 48 bytes of the message

#define WEB_PORT 80
#define ACCESS_POINT	1
#define WIFI_CLIENT		0

#ifndef TELEGRAM_BOT
	#define TELEGRAM_BOT DISABLE
#endif
#if TELEGRAM_BOT == ENABLE 
	#define EN_BOT 
	#include <ESP8266TelegramBOT.h>
#endif 

//
// Message for  different Language
// 
#define ITALIANO
#ifdef ITALIANO
	#define	BASE_COSTRUCTOR 	"cosruttore Base!"
	#define OVERLOAD_COSTRUCTOR "Overload Costruttore!"
	#define AP_SERVER_CREATE	"Crea Access Point!"
	#define AP_CLIENT_CREATE 	"Crea WiFi Client!"
	#define NOT_CONNECTED		"Non Connesso!"
	#define SAVE_AND_REBOOT		"Salvo e Riavvio dispositivo!"
	#define NO_WEB_PAGE			"Pagina non Trovata"
	#define NO_NETWORKS			"Reti WiFi non trovate!"
#endif



#include <WiFiComunication.h>
#include <Service_NTP.h>
#include <Scheduler.h>
#include <WifiScanNetwork.h>
#include <WiFiMem.h>
#include <Service_NTP.h>
#include <drivers.h>
#include <cstring>


#ifndef ESP_8266_01
	#include <ESP8266HTTPUpdateServer.h>
#endif

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#ifdef __AVR__
 #include <avr/io.h>
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#else
 #define PROGMEM
#endif

#endif
