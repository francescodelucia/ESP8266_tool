/*
 * This "#define"s enable the driver of Hardware you want use inside your project
 * */
#define LM75A_TEMP_SENSOR ENABLE



#include <WifiTool.h>

WiFiTool *tool;

/* Routie for Read data from Sensor DHTxx family 
 * */

void GetLocalTemp(){
  char temp[32];
  long data;
  tool->GetDriversData(LM75_CHIP,(void*)&data);
  sprintf(temp,"Temp casa %dC\n",data);
  Serial.printf(temp);
  
  
}


void setup(void) {
  
  tool = new WiFiTool(); 
  /*
    All routine external are added to internal Framework Scheduler
    This routine (Orologio) are called every 1000ms (1sec) 
  */
  tool->AddEvent((void*)GetLocalTemp,1000);
}

void loop() {
  tool->HandlServerEvent(); 
}
