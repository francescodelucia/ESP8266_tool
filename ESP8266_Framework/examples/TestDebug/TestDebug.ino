#include <WifiTool.h>


WiFiTool *tool;

void setup(void)
{  
    tool = new WiFiTool();
  
}

void loop(void)
{
  tool->HandlServerEvent();
}
