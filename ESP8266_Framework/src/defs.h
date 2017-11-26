#ifndef _DEFINITIONS_
#define  _DEFINITIONS_


#define ENABLE		1
#define DISABLE		0

#define LM75A_TEMP_SENSOR	DISABLE
#define TELEGRAM_BOT		DISABLE
#define DTH11_TEMP_SENSOR	ENABLE


#define _BAUD_ 115200

#define _WIFICOM_DEBUG_
#define _WIFIMEM_DEBUG_
#define _WIFI_DEBUG_

//#define EN_BOT

#define _NTP_DEBUG_
#define UDP_PORT 			2390 // local port to listen for UDP packets
#define NTP_SERVER_			"0.it.pool.ntp.org"
#define NTP_PACKET_SIZE 	48	// NTP time stamp is in the first 48 bytes of the message

#define WEB_PORT 80
#define ACCESS_POINT	1
#define WIFI_CLIENT		0

#if LM75A_TEMP_SENSOR == ENABLE 
	#define LM75_CHIP	0x00
	#include <LM75.h> 
#endif 
#if DTH11_TEMP_SENSOR == ENABLE
	#define DTH11_SENSOR 0x01
	#define DTH11_PORT	 14
	#include <dht11.h>
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
#endif
