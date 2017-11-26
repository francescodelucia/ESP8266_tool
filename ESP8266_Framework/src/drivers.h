#ifndef DRIVERS_H
#define DRIVERS_H
#include "defs.h"

#if LM75A_TEMP_SENSOR == ENABLE 
		LM75A *lm75=NULL;
#endif 
#if DTH11_TEMP_SENSOR == ENABLE
		DTH11 *dth11=NULL;
#endif
void readData(){
/*#if LM75A_TEMP_SENSOR == ENABLE
	if (driverType == LM75_CHIP){  
		lm75 = new LM75A(); 
		//return (void*)this->lm75;  
	}
#endif*/
#if DTH11_TEMP_SENSOR == ENABLE
	if (dth11 != NULL){  
		dth11->read(DTH11_PORT); 
		
		//return (void*)this->dth11;  
	}
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
		
		void InitDriver(int driverType)
		{

#if LM75A_TEMP_SENSOR == ENABLE
			if (driverType == LM75_CHIP){  
				lm75 = new LM75A(); 
			}
#endif
#if DTH11_TEMP_SENSOR == ENABLE
			if (driverType == DTH11_SENSOR){  
				dth11 = new DTH11(); 
			}
#endif
			this->driverScheduler->AddEvent((void*)readData,10000);
		}
		void GetDataDriver(int driverType,void* data)
		{
#if LM75A_TEMP_SENSOR == ENABLE
			if (driverType == LM75_CHIP){  
				return lm75->getData();  
			}
#endif		
#if DTH11_TEMP_SENSOR == ENABLE
			if (driverType == DTH11_SENSOR){  
				dth11->getData(data);
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
