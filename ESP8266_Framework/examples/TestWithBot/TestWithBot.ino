#include <WifiTool.h>

WiFiTool *tool;
ESP8266WebServer *myserver;

void pippo()
{
   tool->GetTelegramBot()->getUpdates(tool->GetTelegramBot()->message[0][1]); 
  for (int i = 1; i < tool->GetTelegramBot()->message[0][0].toInt() + 1; i++)      {
    tool->GetTelegramBot()->sendMessage(tool->GetTelegramBot()->message[i][4], tool->GetTelegramBot()->message[i][5], "");
  }
  tool->GetTelegramBot()->message[0][0] = "";   // All messages have been replied - reset new messages
}

#define TOKEN "TOKEN_BOT_ID"
#define NAME "TOKEN_NAME"
#define USERNAME "TOLEN_USERNAME"


void setup(void) {
  tool = new WiFiTool();
  myserver = tool->server;
  tool->SetTelegramBOT(TOKEN,NAME,USERNAME,(void*)pippo);
}

void loop() {
  tool->HandlServerEvent();   
}
