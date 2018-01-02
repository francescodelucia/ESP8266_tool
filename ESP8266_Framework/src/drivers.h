#ifndef DRIVERS_H
#define DRIVERS_H
#include <defs.h>


void readData(){

#if DTH11_TEMP_SENSOR == ENABLE
	dth11->read(DTH11_PORT); 
#endif
#if LM75A_TEMP_SENSOR == ENABLE
	lm75->read(); 
#endif
}
void readWebData(){
#if METEO_WEB_DATA == ENABLE  
	meteo->read(); 
#endif
}

class Drivers {
	  private:
		Scheduler *driverScheduler;

#if TELEGRAM_BOT == ENABLE
		TelegramBOT *teleBOT;
#endif
	  public:
	    Drivers()
	    {
			this->driverScheduler = new Scheduler();
		}
		
		void poolingDriver(){this->driverScheduler->RunScheduler();}
		
		void InitDriver()
		{
			readData();
			this->driverScheduler->AddEvent((void*)readWebData,180000);
			this->driverScheduler->AddEvent((void*)readData,10000);
		}
		void GetDataDriver(int driverType,void* data)
		{
#if LM75A_TEMP_SENSOR == ENABLE
			if (driverType == LM75_CHIP){  
				lm75->getData();  
			}
#endif		
#if DTH11_TEMP_SENSOR == ENABLE
			if (driverType == DTH11_SENSOR){  
				dth11->getData(data);
			}
#endif		
#if METEO_WEB_DATA == ENABLE
			if (driverType == METEO_WEB){  
				meteo->getData(data);
			}
#endif
		}
		void DisplayCommand(int dispalyCommand,void* data,void* optional){
#if DISPLAY_HT1632 == ENABLE
			if (dispalyCommand == HT1632_FADEDOWN){
				 ht1632->fadeDown();
			}else if (dispalyCommand == HT1632_SCROLL){
				ht1632->displayScrollingLine((char*)data,(int)optional);
			}else if (dispalyCommand == HT1632_BRIGHT){
				ht1632->setBrightness((int)data);
			}else if (dispalyCommand == HT1632_FADEUP){
				ht1632->fadeUp();
			}else if (dispalyCommand == HT1632_EFFECT){
				ht1632->displayEffect((char*)data,(int)optional);
			}else if (dispalyCommand == HT1632_CLOCK){
				ht1632->displayClock((char*)data,(int)optional);
			}
#endif
		}
#if TELEGRAM_BOT == ENABLE
		TelegramBOT* GetTelegramBot(String token, String name, String username, void* callbackRoutine)
		{	
			this->teleBOT = new TelegramBOT(token, name, username);
			this->teleBOT->begin();
			this->driverScheduler->AddEvent(callbackRoutine,1000);
			return this->teleBOT;
		}
#endif
		
		
};
#endif
