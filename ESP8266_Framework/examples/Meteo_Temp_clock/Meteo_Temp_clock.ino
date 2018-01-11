/*
 * This "#define"s enable the driver of Hardware you want use inside your project
 * */
#define DTH11_TEMP_SENSOR ENABLE
#define METEO_WEB_DATA ENABLE
#define DISPLAY_HT1632 ENABLE
/*
 * This #define , you need change if you want read Meteo Data of Your City
 * */
#define LOC_ "YOUR_CITY_NAME_EX_ROME"
/*
 * Just this include you need call for use this project, all data managment,
 * Wifi Manager, OTA rouitine, Drivers and other are inside the Framework, 
 * */
#include <WifiTool.h>

WiFiTool *tool;

/* Routie for Read data from Sensor DHTxx family 
 * */
void GetLocalTemp(){
  char temp[32];
  int data[2];
  tool->GetDriversData(DTH11_SENSOR,(void*)&data);
  sprintf(temp,"Temp casa %dC Umid %d%",data[1],data[0]);
  
  /* 
   * I use display of Sure Electronics HT1632 for show data
   * */
  tool->SendDisplayCommand(HT1632_BRIGHT,(void*)10,NULL);
  tool->SendDisplayCommand(HT1632_FADEUP,NULL,NULL);
  tool->SendDisplayCommand(HT1632_SCROLL,(void*)&temp[0], 0);                                                
}
/*
 * Routine for chapture data from Web (openweathermap)
 * 
 * Note. Remeber to subscribe your personal API TOKEN openweathermap.org to 
 * use this feature
 * 
 * */
int iMsg = 0;
void GestMeteo(){
  struct meteo_data mtd;
        
        
  tool->GetDriversData(METEO_WEB,(void*)&mtd);
        
  if(strlen(mtd.temp)>0){          
    char temp[32];
    switch(iMsg){
      case 0:{
        sprintf(temp,"%s %sC",mtd.localita,mtd.temp);  
        break;
      }
      case 1:{
        sprintf(temp,"%s Min %sC - Max %sC",mtd.localita,mtd.temp_min,mtd.temp_max);                                     
        break;
      }
      case 2:{      
        sprintf(temp,"%s Umidita %s%",mtd.localita,mtd.humid);             
        break;
      }
      case 3:{
        sprintf(temp,"%s previsioni %s - %s",mtd.localita,mtd.weath,mtd.description);                           
        break;
      }
     }
    /* 
     * I use display of Sure Electronics HT1632 for show data
	*/
     tool->SendDisplayCommand(HT1632_FADEDOWN,NULL,NULL);
     tool->SendDisplayCommand(HT1632_BRIGHT,(void*)10,NULL);           
     tool->SendDisplayCommand(HT1632_SCROLL,(void*)&temp[0], 0);              
     iMsg ++;
     if(iMsg == 3){iMsg=0;}
  }
  GetLocalTemp();
}

/*
 * Routine for Show Time Clock
 * 
 * */

char f =':'; 
void Orologio(){
  int tSec_ = tool->GetLTZSeconds();
  int tMin_ = tool->GetLTZMinutes();
  int tHou_ = tool->GetLTZHours();
  char Time[32];
  if (f == ':'){f = ' ';}else{f = ':';}   
   sprintf(Time,"%02d%c%02d",tHou_,f,tMin_);         
   tool->SendDisplayCommand(HT1632_CLOCK,(void*)&Time[0],0);
}


void setup(void) {
  char deviceIp[32];   
  tool = new WiFiTool();
  tool->SendDisplayCommand(HT1632_BRIGHT,(void*)10,NULL);
  tool->SendDisplayCommand(HT1632_SCROLL,(void*)"ClockNet v0.9 - DLF", 0);
  tool->SendDisplayCommand(HT1632_EFFECT,(void*)"Conn..", 0);
  sprintf(deviceIp,"Ip - %s",(char*)((String)(tool->GetIp())).c_str());
  tool->SendDisplayCommand(HT1632_BRIGHT,(void*)10,NULL);
  tool->SendDisplayCommand(HT1632_SCROLL,(void*)&deviceIp[0], 0);
  /*
   * All routine external are added to internal Framework Scheduler 
   * */
  tool->AddEvent((void*)Orologio,1000);
  tool->AddEvent((void*)GestMeteo,60000);
}


void loop() {
  tool->HandlServerEvent();  
}

