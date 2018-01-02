#ifndef _WIFI_TOOL_
#define _WIFI_TOOL_
#include <defs.h>

extern "C" {
#include "user_interface.h"
}

extern void localSeconds();
typedef void (*httpFunction)();
typedef void(*botCallBack)(void);

int LogTimeEvent = 0;



void Diagnostic_Comunication()
{	
#ifdef _WIFI_DEBUG_
	Serial.println("Diagnostic_Comunication: ");
#endif
	if(WiFi.status()!= WL_CONNECTED){system_restart();}
}

class WiFiTool{
	private:
		int _personalRootWebPage = 0;
		WiFiMEM* wimem = NULL;
		WiFiComunication* wificom = NULL; 
		ESP8266_ScanNetwork *scan = NULL;
		//botCallBack bCall=NULL;
		Scheduler *SysScheduler=NULL;
		Scheduler *UserScheduler=NULL;
		//int Bot_mtbs = 1000; //mean time between scan messages
		//long Bot_lasttime;
		Drivers *drivers =NULL;;
#ifdef EN_BOT
		TelegramBOT *teleBOT = NULL;
#endif		   

/*
 * Implement te remote Updater, but on ESP model with low Memory onboard this option doesn't work,
 * and in every case you need consider the maximun Maximun memory size of our programm not exceed of 
 *  50% of total memory, just the Framework it is 25% of memory on device with 1M memory size.
 * */
#ifndef ESP_8266_01
		ESP8266HTTPUpdateServer *httpUpdater = NULL;
#endif

		Service_NTP *sntp = NULL;
		int WifiClient;
		//const char* host = "OTA-LEDS";
		
	protected:
		
	public:
		ESP8266WebServer *server= NULL;
		
		/*
		 * This Routine(WebPage) save the system parameter on internal memory   
		 */
		void storeData(){     
			/*
			Set to default no NTP Server 
			 */
			this->wimem->SetNTP_ON(0);
			for (uint8_t i=0; i<this->server->args(); i++){
				if(this->server->argName(i)== "CLIENT_SSID")
				{
#ifdef _WIFI_DEBUG_
					Serial.println("B_CLIENT_SSID");
#endif
					const int k = this->server->arg(i).length();
					if(k>0){
						char cSSID[k];
						std::strcpy(cSSID,this->server->arg(i).c_str());      
						this->wimem->SetClientSSID(cSSID);
					}
#ifdef _WIFI_DEBUG_
					Serial.println("E_CLIENT_SSID");
#endif
				}
				else if(this->server->argName(i)== "CLIENT_PWD")
				{
#ifdef _WIFI_DEBUG_
					Serial.println("B_CLIENT_PWD");
#endif
					const int k = this->server->arg(i).length();
					if(k>0){
						char cPWD[k];
						std::strcpy(cPWD,this->server->arg(i).c_str());      
						this->wimem->SetClientPWD(cPWD);
					}
#ifdef _WIFI_DEBUG_
					Serial.println("E_CLIENT_PWD");
#endif
				}
				else if(this->server->argName(i)== "SERVER_PWD")
				{
#ifdef _WIFI_DEBUG_
					Serial.println("B_SERVER_PWD");
#endif
					const int k = this->server->arg(i).length();
					if(k>0){
						char sPWD[k];
						std::strcpy(sPWD,this->server->arg(i).c_str());      
						this->wimem->SetServerPWD(sPWD);
					}
#ifdef _WIFI_DEBUG_
					Serial.println("E_SERVER_PWD");
#endif

				}
				else if(this->server->argName(i)== "SERVER_SSID")
				{
#ifdef _WIFI_DEBUG_
					Serial.println("B_SERVER_SSID");
#endif
					const int k = this->server->arg(i).length();
					if(k>0){
						char sSSID[k];
						std::strcpy(sSSID,this->server->arg(i).c_str());      
						this->wimem->SetServerSSID(sSSID);
					}
#ifdef _WIFI_DEBUG_
					Serial.println("E_SERVER_SSID");
#endif
				}
				else if(this->server->argName(i)== "NTP")
				{
#ifdef _WIFI_DEBUG_
					Serial.println("B_NTP");
#endif
					const int k = this->server->arg(i).length();
					if(k>0){
						char cNTP[k]; 
						std::strcpy(cNTP,this->server->arg(i).c_str());      
						this->wimem->SetNTP(cNTP);
					}
#ifdef _WIFI_DEBUG_
					Serial.println("E_NTP");
#endif
			}
			else if(this->server->argName(i)== "NTP_ON")
			{						
#ifdef _WIFI_DEBUG_
					Serial.println("B_NTP_ON");
#endif
				const int k = this->server->arg(i).length();
				if(k>0){
					if(this->server->arg(i) == "On")
					{this->wimem->SetNTP_ON(1);}
					else
					{this->wimem->SetNTP_ON(0);}
				}
#ifdef _WIFI_DEBUG_
					Serial.println("E_NTP_ON");
#endif
			}							   
			else if(this->server->argName(i)== "SERVER_PORT")
			{
#ifdef _WIFI_DEBUG_
				Serial.println("B_SERVER_PORT");
#endif
				const int k = this->server->arg(i).length();
				if(k>0){
					char sSPORT[k];
					std::strcpy(sSPORT,this->server->arg(i).c_str());        
					uint16_t _sp = (uint16_t)atoi(sSPORT);
					Serial.println(_sp);	
					this->wimem->SetServerPort(_sp);
				}
#ifdef _WIFI_DEBUG_
				Serial.println("E_SERVER_PORT");
#endif
			  
			}
			else if(this->server->argName(i)== "NTP_TIME_ZONE")
			{
#ifdef _WIFI_DEBUG_
				Serial.println("B_NTP_TIME_ZONE");
#endif
				const int k = this->server->arg(i).length();
				if(k>0){
					char sNTPTZ[k];
					std::strcpy(sNTPTZ,this->server->arg(i).c_str());        
					int8_t _ntz = (int8_t)atoi(sNTPTZ);
					Serial.println(_ntz);
					this->wimem->SetNTP_Tyme_Zone(_ntz);
				}
#ifdef _WIFI_DEBUG_
				Serial.println("E_NTP_TIME_ZONE");
#endif
			}
			else if(this->server->argName(i)== "COMM_BAUD")
			{
#ifdef _WIFI_DEBUG_
				Serial.println("B_COMM_BAUD"); 
#endif
				const int k = this->server->arg(i).length();
				if(k>0){
					char sCBAUD[k];
					std::strcpy(sCBAUD,this->server->arg(i).c_str());  
					uint32_t _cb = (uint32_t)atoi(&sCBAUD[0]);
					this->wimem->SetCommBaud(_cb);
				}
#ifdef _WIFI_DEBUG_
				Serial.println("E_COMM_BAUD");
#endif
			}
		}
#ifdef _WIFI_DEBUG_
		  Serial.println("END_PROCESS_DATA");
#endif
		  this->wimem->Save();
		  
		  delay(100);
		  
		   String out =  "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
			out +="<style>#myProgress{position: relative;width: 100%;height: 30px;background-color: #ddd;}";
			out += "#myBar{position: absolute;width: 1%;height: 100%;background-color: #0000ff;}</style>";
			out +="</head><body style='max-width:1440px'><center><h1>" + String(SAVE_AND_REBOOT) + "</h1><div id='myProgress'><div id='myBar'></div>";
			out +="</div><br></center><script>var elem = document.getElementById('myBar');var width = 1;";
			out +="var id = setInterval(frame, 70);function frame(){if(width >= 100){clearInterval(id);";
			out +="window.location = './';}else{width++;elem.style.width = width + '%';}}</script></body></html>";
			this->server->send(200, "text/html", out);
			delay(200);
			system_restart();
		 }
		/*
		 * This routine handle web page not Found
		 * */
		void handleNotFound(){
			String message = NO_WEB_PAGE;
			message += "URI: ";
			message += this->server->uri();
			message += "\nMethod: ";
			message += (this->server->method() == HTTP_GET)?"GET":"POST";
			message += "\nArguments: ";
			message += this->server->args();
			message += "\n";
			for (uint8_t i=0; i<this->server->args(); i++){
				message += " " + this->server->argName(i) + ": " + this->server->arg(i) + "\n";
			}
			this->server->send(404, "text/plain", message);  
		}
		/*
		 * This is the default web Pages cutomizable to the external class
		 * */
		void itWork() {  
		  
			int m = 0 ; int h = 0 ; int s = 0; String ntp = "";
			if(this->sntp!=NULL){
				m = this->sntp->GetLTZMinutes();
				h = this->sntp->GetLTZHours();
				s = this->sntp->GetLTZSeconds();
				ntp = this->sntp->GetNTPServer();
			}
			char tmp[800 + 17 ];   
			memset(tmp,0,800 + 17); 
			sprintf ( tmp,
			  "<html><head>\
			   <META HTTP-EQUIV='refresh' CONTENT=3>\
			  <title>ESP8266 device OK</title>\
			  <meta name='viewport' content='width=device-width, initial-scale=1'><style>\
			   body { background-color: #FFFFFF; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\
			  </style></head><body  style='max-width:1440px'>\
			  <br><br><br><br>\
			  <CENTER>\
			  <br>ESP8266 Device information\
			  <br>Device MAC:%s\
			  <br>Ntp Server %s\
			  <br>Time %i:%i:%i\
			  <br><H1>It Work!</H1>\
			  <br><a href='./manager' class='button'>Settings</a>\
			  </CENTER>\
			  </body>\
			  </html>",this->wificom->MAC_char,ntp.c_str(),h,m,s);
			  
			this->server->send(200, "text/html", tmp);
		}
		
		/*
		 * THis routine/webpage create javascript for Amministrative Page
		 * */
		String SystemInformation()
		{
			String tmp;
			tmp =  "Flash memory:     " + String(ESP.getFlashChipSize())+" Bytes.\n";
			tmp += "Free heap memory: " + String(ESP.getFreeHeap()) + " Bytes.\n";
			tmp += "Chip speed:       " + String(ESP.getFlashChipSpeed())+" Hz.\n";
			return tmp;
		}
		void JavaScripter()
		{
			String tmp;
			if (this->wimem->GetCRCError() == 1)
			{
				tmp = "var data = {MAC:'" + String(this->wificom->MAC_char) + "',SERVER_SSID:'ESP8266"; 
				tmp += String(this->wificom->MAC_char) + "',SERVER_PWD:'";
				tmp += String(this->wificom->MAC_char) + "',SERVER_PORT:'80',CLIENT_PWD:'',NTP:'ntp1.inrim.it'};";
			}else{
				tmp = "var data = {MAC:'" + String(this->wificom->MAC_char) + "',SERVER_SSID:'" + String(this->wimem->GetServerSSID()); 
				tmp += "',SERVER_PWD:'" + String(this->wimem->GetServerPWD()) + "',SERVER_PORT:'" +  String(this->wimem->GetServerPort());
				tmp += "',CLIENT_PWD:'" + String(this->wimem->GetClientPWD()) + "',NTP:'" + String(this->wimem->GetNTP())  + "'};";
					
			}
			tmp += "var aSSID=[' '," + this->scan->EnumNetwork() + "];";
			tmp += "var aLTZ =['+12','+11','+10','+09','+08','+07','+06','+05','+04','+03','+02','+01','00','-01','-02','-03','-04','-05','-06','-07','-08','-09','-10','-11','-12'];";
			tmp += "var aNTP =['On','Off'];";
			
			if (this->wimem->GetCRCError() == 1)
			{
				tmp +="var currentSSID =' ';var currentTZ='+01';var currentNTP_SEL = 'On';";
			}else{
				int8_t x = this->wimem->GetNTPTymeZone();
				int8_t abs_x = (x + (x >> 8)) ^ (x >> 8);
				if ( abs_x >= 10)
				{
					tmp += "var currentTZ='" + (x >= 0 ? String("+"):(String("-")) + String(abs_x)) + "';";
				}
				else
				{
					tmp += "var currentTZ='" + (x >= 0 ? String("+0"):(String("-0")) + String(abs_x)) + "';";
				}
				tmp +="var currentSSID ='" + String(this->wimem->GetClientSSID()) + "';";
				tmp +="var currentNTP_SEL ='" +  (1 == (int)this->wimem->GetNTP_ON() ? String("On") :  String("Off"))   + "';";
			}

			tmp +="var inputs = Array.prototype.slice.call(document.querySelectorAll('form input'));";
			tmp +="Object.keys(data).map(function (dataItem){";
			tmp +="inputs.map(function (inputItem){return (inputItem.name === dataItem) ? (inputItem.value = data[dataItem]) : false;});});";
			tmp +="selectCS = document.getElementById('CLIENT_SSID');";
			tmp +="for( _SSID in aSSID ){selectCS.add(new Option(aSSID[_SSID]));};";
			tmp +="selectCS.value = currentSSID;selectNTP = document.getElementById('NTP_TIME_ZONE');";
			tmp +="for( _LTZ in aLTZ ){selectNTP.add( new Option( aLTZ[_LTZ]));};selectNTP.value = currentTZ;";
			tmp +="selectNTP_CB = document.getElementById('NTP_ON');for( _NTP in aNTP ){selectNTP_CB.add( new Option( aNTP[_NTP]));};";
			tmp +="selectNTP_CB.value = currentNTP_SEL;";

			tmp +="function LoadDefaultData(){";
			tmp += "var data = {MAC:'" + String(this->wificom->MAC_char) + "',SERVER_SSID:'ESP8266" + String(this->wificom->MAC_char) + 
						"',SERVER_PWD:'" + String(this->wificom->MAC_char) +  "',SERVER_PORT:'80',CLIENT_PWD:'',NTP:'ntp1.inrim.it'};";
			tmp +="var inputs = Array.prototype.slice.call(document.querySelectorAll('form input'));Object.keys(data).map(function(dataItem){";
			tmp +="inputs.map(function (inputItem){return (inputItem.name === dataItem) ? (inputItem.value = data[dataItem]) : false;});});}";
			 
			this->server->send( 200, "text/javascript; charset=UTF-8", tmp);
			
		}
		
		/*
		 * THis routine/webpage create HTML with reboot
		 * */
		void deviceReboot() {    
			String out =  "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
				out +="<style>#myProgress{position: relative;width: 100%;height: 30px;background-color: #ddd;}";
				out += "#myBar{position: absolute;width: 1%;height: 100%;background-color: #0000ff;}</style>";
				out +="</head><body style='max-width:1440px'><center><h1>Reboot</h1><div id='myProgress'><div id='myBar'></div>";
				out +="</div><br></center><script>var elem = document.getElementById('myBar');var width = 1;";
				out +="var id = setInterval(frame, 150);function frame(){if(width >= 100){clearInterval(id);";
				out +="window.location = './';}else{width++;elem.style.width = width + '%';}}</script></body></html>";
			this->server->send(200, "text/html", out);
			system_restart();
		}
		
		/*
		 * THis routine/webpage create HTML device manager
		 * */
		void deviceManager() {    

			String out =  "<html><head><title>ESP8266 device information</title>";
			out += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
			out += "<style>body{ background-color: #FFFFFF; font-family: Arial, Helvetica, Sans-Serif; Color: #0000FF; }</style>";
			out += "</head><body style='max-width:1440px'><br><CENTER>ESP8266 Device Configuration<br><br></CENTER>";
			out += "<form action='./store' method='GET'><CENTER><TABLE>";
			out += "<tr><td>Device MAC:</td><td><input type='textbox' name='MAC' readonly/></td></tr>";
			out += "<tr><td>Wifi AP SSID</td><td><input type='textbox' name='SERVER_SSID'/></td></tr>";
			out += "<tr><td>Wifi AP Password</td><td><input type='textbox' name='SERVER_PWD'/></td></tr>";
			out += "<tr><td>Wifi AP Port</td><td><input type='number' pattern='[0-9]' name='SERVER_PORT'/></td></tr>";
			out += "<tr><td>Wifi Client SSID</td><td><select id='CLIENT_SSID' name='CLIENT_SSID'/></td></tr>";
			out += "<tr><td>Wifi Client Password</td><td><input type='textbox'name='CLIENT_PWD'/></td></tr>";
			out += "<tr><td>NTP Server</td><td><input type='textbox' name='NTP'/></td></tr>";
			out += "<tr><td>NTP time zone</td><td><select id='NTP_TIME_ZONE' name='NTP_TIME_ZONE' /></td></tr>";
			out += "<tr><td>NTP</td><td><select id='NTP_ON' name='NTP_ON' ></select></td></tr>";
			out += "</TABLE></CENTER><CENTER><br><input type='submit' value='Salva Modifiche'>";
			out += "</form><input type='button' value='Annulla'></CENTER>";
			out += "<CENTER><LABEl>Utility<LABEL></td><td>&nbsp;</td></tr>";
#ifndef ESP_8266_01
			out += "<CENTER><input type='button' value='Aggiorna Firmware' onclick='window.location =\"./update\";' style='width:350px'></<CENTER>";
#endif
			out += "<CENTER><input type='button' value='Load Default Configuration' onclick='LoadDefaultData();' style='width:350px'></<CENTER>";
			out += "<CENTER><input type='button' value='Reboot' onclick='window.location =\"./reboot\";' style='width:350px'></<CENTER>";
			out += "</CENTER><CENTER><PRE>" + this->SystemInformation() + "</PRE></CENTER></body><script type='text/javascript' src='./jscripter'></script></html>";
			  
			this->server->send(200, "text/html", out);
		}
	
	public:	
		/*
		 * Costructor overload  where choose if you want customize Root page
		 * */
		
		WiFiTool(int PersonalWebRoot /* 1 = YES ; 0 = NO */):WiFiTool(){
			this->_personalRootWebPage = PersonalWebRoot;
		}
		WiFiTool(){
			ESP.eraseConfig();
			delay(500);
			
			WifiClient = ACCESS_POINT;

			Serial.begin(_BAUD_);   
#ifdef _WIFI_DEBUG_
			Serial.println("#######################################################");
			Serial.println("Esp8266 Framework by Francesco De lucia ver 0.180102");
			Serial.println("This Messages are Debug comunicatio and system Messages");  
			Serial.println("If you want remove it. Comment following lines:");  
			Serial.println("\n#define _WIFICOM_DEBUG_");
			Serial.println("#define _WIFIMEM_DEBUG_");
			Serial.println("#define _WIFI_DEBUG_");
			Serial.println("#define _NTP_DEBUG_\n");
			Serial.println("on file defs.h");
			Serial.println("#######################################################");  
#endif	
			this->wimem = new WiFiMEM(); 
#ifndef ESP_8266_01
			this->httpUpdater = new ESP8266HTTPUpdateServer();
#endif
		
			this->SysScheduler = new Scheduler();
			this->UserScheduler = new Scheduler();
			
			this->scan = new ESP8266_ScanNetwork();
			this->drivers = new Drivers();
			this->drivers->InitDriver();
			if(this->wimem->GetCRCError() == 1)
			{
				this->server = new ESP8266WebServer(WEB_PORT);
			}
			else
			{
				if((int)this->wimem->GetServerPort()<1)
				{
					this->server = new ESP8266WebServer(WEB_PORT);
				}else{
					this->server = new ESP8266WebServer((int)this->wimem->GetServerPort());
				}
			}
  
#ifdef _WIFI_DEBUG_
			Serial.printf("SSID %s PWD %s\n",this->wimem->GetClientSSID(),this->wimem->GetClientPWD());  
#endif
			if(this->wimem->GetCRCError() == 1)
			{
				this->wificom = new WiFiComunication(this->wimem->GetClientSSID(),this->wimem->GetClientPWD());
			}
			else
			{
				this->wificom = new WiFiComunication(this->wimem->GetClientSSID(),this->wimem->GetClientPWD(),
					this->wimem->GetServerSSID(),this->wimem->GetServerPWD());
			}
			if(this->wificom->CreateWFClient() == ACCESS_POINT)
			{         
				this->WifiClient = ACCESS_POINT;       
				if ( this->_personalRootWebPage == 0 )
				{ 
					this->server->on("/", [this](){itWork();});
				}
				this->server->on("/store", [this](){this->storeData();});
				this->server->on("/reboot", [this](){this->deviceReboot();});
				this->server->on("/jscripter", [this](){this->JavaScripter();});
				this->server->on("/manager", [this](){this->deviceManager();});
				this->server->onNotFound([this](){handleNotFound();});
#ifndef ESP_8266_01				
				this->httpUpdater->setup(this->server);				
#endif
				/**Serial.printf("ntp %i\n",wimem->GetNTP_ON());
				if(this->wimem->GetNTP_ON()==1){
					this->sntp = new Service_NTP(this->wimem->GetNTP());
				}*/
				this->server->begin();
			}
			else //if(wificom->CreateWFClient() == 0)
			{
				this->WifiClient = WIFI_CLIENT;
				if ( this->_personalRootWebPage == 0 )
				{ 
					this->server->on("/",[this](){itWork();});
				}
				this->server->on("/store", [this](){storeData();});
				this->server->on("/reboot", [this](){this->deviceReboot();});
				this->server->on("/jscripter", [this](){this->JavaScripter();});
				this->server->on("/manager", [this](){this->deviceManager();});

				this->server->onNotFound([this](){handleNotFound();});	
#ifndef ESP_8266_01				
				this->httpUpdater->setup(this->server);				
#endif			
				this->SysScheduler->AddEvent((void*)Diagnostic_Comunication,60000);
				
				Serial.printf("ntp %i\n",wimem->GetNTP_ON());
				
				if(this->wimem->GetNTP_ON()==1){
					this->sntp = new Service_NTP(this->wimem->GetNTP());
					this->sntp->SetLocalTimeZone((int)this->wimem->GetNTPTymeZone());
					this->SysScheduler->AddEvent((void*)localSeconds,1000);
				}
				this->server->begin();
			}	
		}
		void AddWebRoot(void* callbackWebRoutine) {			
			if(this->_personalRootWebPage==1)
			{			
				this->server->on("/",(void(*)())callbackWebRoutine);	
			}
		} 
		void AddWebPage(void* callbackWebRoutine,char* webpagename) {						
			this->server->on(webpagename,(void(*)())callbackWebRoutine);	
		} 
		
		void AddPostData(void* callbackWebRoutine,char* webpagename) {									
			this->server->on(webpagename, HTTP_POST,(void(*)())callbackWebRoutine);
		} 
		void SetLocalTimeZone(int ltz)
		{
			if (this->sntp != NULL)
			{
				this->sntp->SetLocalTimeZone(ltz);
			}
		}	
		String GetIp()
		{						
			return this->wificom->GetIp();
		}
		char* GetNTP(){		
			return (char*)this->sntp->GetNTPServer().c_str();
		}
		char* GetClientSSID()
		{
			return this->wimem->GetClientSSID();
		}
		char* GetServerSSID()
		{
			return this->wimem->GetServerSSID();
		}
		int GetUTCMinutes(){
			return this->sntp->GetUTCMinutes();
		}
		int GetUTCHours(){
			return this->sntp->GetUTCHours();
		}
		int GetUTCSeconds(){
			return this->sntp->GetUTCSeconds();
		}
		int GetLTZMinutes(){
			return this->sntp->GetLTZMinutes();
		}
		int GetLTZHours(){
			return this->sntp->GetLTZHours();
		}
		int GetLTZSeconds(){
			return this->sntp->GetLTZSeconds();
		}
		int IsWifiClient()
		{
			return  WifiClient;
		}
		int AddEvent(void *pCallBack,int millisecond_event)
		{
			return this->UserScheduler->AddEvent(pCallBack,millisecond_event);
		}
		void DisableEvent(int ID_EVENT)
		{
			this->UserScheduler->DisableEvent(ID_EVENT);
		}
		void DisableAllEvent()
		{
			this->UserScheduler->DisableAllEvent();
		}
		void RestoreAllEvent()
		{
			this->UserScheduler->RestoreAllEvent();
		}
		void DisableAllSystemEvent()
		{
			this->SysScheduler->DisableAllEvent();
		}
		void RestoreAllSystemEvent()
		{
			this->SysScheduler->RestoreAllEvent();
		}
		void ModifyEvent(int ID_EVENT,void *pCallBack,int millisecond_event)
		{
			this->UserScheduler->ModifyEvent(ID_EVENT,pCallBack,millisecond_event);
		}
		void HandlServerEvent()
		{			
			this->server->handleClient();
			if (this->WifiClient != ACCESS_POINT){
				this->drivers->poolingDriver();
				this->SysScheduler->RunScheduler();
				this->UserScheduler->RunScheduler();
			}
		}
		void s_print(char *msg)
		{
			Serial.println(msg);
		}
		int* GetDriversData(int dType,void* data)
		{			
			this->drivers->GetDataDriver(dType,data);
		}
		void SendDisplayCommand(int dType,void* data,void* optional){
			this->drivers->DisplayCommand(dType,data,optional);
		}
		/*
		TelegramBOT *SetTelegramBOT(String token, String name, String username, void* callbackRoutine){	
			this->teleBOT = new TelegramBOT(token, name, username);
			this->teleBOT->begin();
			this->SysScheduler->AddEvent(callbackRoutine,1000);
			return this->teleBOT;
		}
		
		TelegramBOT *GetTelegramBot(){
			return this->teleBOT;
		}
		*/			
};
#endif
