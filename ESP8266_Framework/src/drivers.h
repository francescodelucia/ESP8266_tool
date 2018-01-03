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
#if DISPLAY_SSD1306 == ENABLE
			Wire.begin(); 
#endif
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
		void DisplayCommand(int displayCommand,void* data,void* optional,void* optional1,void* optional2){
#if DISPLAY_HT1632 == ENABLE
			if (displayCommand == HT1632_FADEDOWN){
				 ht1632->fadeDown();
			}else if (displayCommand == HT1632_SCROLL){
				ht1632->displayScrollingLine((char*)data,(int)optional);
			}else if (displayCommand == HT1632_BRIGHT){
				ht1632->setBrightness((int)data);
			}else if (displayCommand == HT1632_FADEUP){
				ht1632->fadeUp();
			}else if (displayCommand == HT1632_EFFECT){
				ht1632->displayEffect((char*)data,(int)optional);
			}else if (displayCommand == HT1632_CLOCK){
				ht1632->displayClock((char*)data,(int)optional);
			}
#endif
			
#if DISPLAY_SSD1306 == ENABLE
			if (displayCommand == SSD1306_INIT){
				SSD1306->init();
			}else if(displayCommand == SSD1306_SET_NORMAL_DISPLAY){
				SSD1306->setNormalDisplay();
			}else if(displayCommand == SSD1306_SET_INVERSE_DISPLAY){
				SSD1306->setInverseDisplay();
			}else if(displayCommand == SSD1306_SEND_CMD){	
				SSD1306->sendCommand(*(unsigned char*)data);
			}else if(displayCommand == SSD1306_SEND_DATA){	
				SSD1306->sendData(*(unsigned char*)data);
			}else if(displayCommand == SSD1306_PAGE_MODE){	
				SSD1306->setPageMode();
			}else if(displayCommand == SSD1306_SET_H_MODE){	
				SSD1306->setHorizontalMode();
			}else if(displayCommand == SSD1306_SET_TEXT_XY){	
				SSD1306->setTextXY((unsigned char)(int)data,(unsigned char)(int)optional);
			}else if(displayCommand == SSD1306_CLEAR){	
				SSD1306->clearDisplay();
			}else if(displayCommand == SSD1306_BRIGHT){	
				SSD1306->setBrightness((unsigned char)(int)data);
			}else if(displayCommand == SSD1306_PUT_CHAR){	
				SSD1306->putChar(*(unsigned char*)data);
			}else if(displayCommand == SSD1306_PUT_STR){	
				SSD1306->putString((const char*)data);
			//}else if(displayCommand == SSD1306_PUT_STR2){
				//SSD1306->putString((String)data);
			}else if(displayCommand == SSD1306_PUT_NUMBER){	
				SSD1306->putNumber(*(long*)data);
			}else if(displayCommand == SSD1306_PUT_FLOAT){
				if(optional!=NULL){SSD1306->putFloat(*(float*)data,*(unsigned char*)optional);}
				else{SSD1306->putFloat(*(float*)data); }
			}else if(displayCommand == SSD1306_DRAW_BMP){
				SSD1306->drawBitmap((unsigned char *)data,(int)optional);
				//drawBitmap(unsigned char *bitmaparray,int bytes);
			}else if(displayCommand == SSD1306_SET_H_SCROLL){
				SSD1306->setHorizontalScrollProperties(*(bool*)data,*(unsigned char*)optional,
										*(unsigned char*)optional1,*(unsigned char*)optional2);
			}else if(displayCommand == SSD1306_SET_SCROLL_ON){
				SSD1306->activateScroll();
			}else if(displayCommand == SSD1306_SET_SCROLL_OFF){
				SSD1306->deactivateScroll();
			}else if(displayCommand == SSD1306_SET_FONT){
				SSD1306->setFont((const uint8_t*)data);
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
