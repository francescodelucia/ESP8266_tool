#include <WifiTool.h>


ESP8266_Tool *tool;

void setup(void)
{  
    tool = new ESP8266_Tool();
  
}

void loop(void)
{
  tool->HandlServerEvent();
}