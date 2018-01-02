#include <WifiTool.h>

WiFiTool *tool;
ESP8266WebServer *myserver;
#define RELE1 0   //GPIO0
#define RELE2 2   //GPIO2
int iRele1;
int iRele2;
char rel1 = '0';
char rel2 = '0';


 /* 
  http://10.0.0.120/data?RELE1=0&RELE2=0  
   */
   
void Data(){
   for (uint8_t i=0; i<myserver->args(); i++){
      if(myserver->argName(i)== "RELE1")
      {        
        const int k = myserver->arg(i).length();
        char chRele1[k];
        std::strcpy(chRele1,myserver->arg(i).c_str());                                     
        iRele1 = atoi (chRele1);
        digitalWrite(RELE1, (iRele1 >= 1) ? HIGH: LOW);
        rel1 = (iRele1 >= 1) ? '1': '0';
      }
      else if(myserver->argName(i)== "RELE2")
      {             
        const int k = myserver->arg(i).length();
        char chRele2[k];
        std::strcpy(chRele2,myserver->arg(i).c_str());                                     
        iRele2 = atoi (chRele2);
        digitalWrite(RELE2, (iRele2 >= 1) ?  HIGH: LOW);
        rel2 = (iRele2 >= 1) ? '1': '0';
      }
   }
    char tmp[50 ]; 
   memset(tmp,0,50);   
    sprintf ( tmp,"R1%c:R2%c",rel1,rel2);        
    tool->server->send(200, "text/html", tmp);       
         
}


void Rele(){
 
   for (uint8_t i=0; i<myserver->args(); i++){
      if(myserver->argName(i)== "RELE1")
      {        
        const int k = myserver->arg(i).length();
        char chRele1[k];
        std::strcpy(chRele1,myserver->arg(i).c_str());                                     
        iRele1 = atoi (chRele1);
        digitalWrite(RELE1, (iRele1 >= 1) ? HIGH: LOW);
        rel1 = (iRele1 >= 1) ? '1': '0';
      }
      else if(myserver->argName(i)== "RELE2")
      {             
        const int k = myserver->arg(i).length();
        char chRele2[k];
        std::strcpy(chRele2,myserver->arg(i).c_str());                                     
        iRele2 = atoi (chRele2);
        digitalWrite(RELE2, (iRele2 >= 1) ?  HIGH: LOW);
        rel2 = (iRele2 >= 1) ? '1': '0';
      }
   }      
   char tmp[800 ]; 
   memset(tmp,0,800);   
   
    sprintf ( tmp,
        "<html><head><title>ESP8266 Connection</title><style>\
        body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
        </style>\
        </head>\
        <body>\
        <CENTER>\
        <h1> ESP8266 Web Server</h1>\
        <H1>RELE' status</H1>\
        <H1>RELE'1:[%c]  RELE'2:[%c]</H1>\
        <p>R1 <a href='\/rele?RELE1=1'><button>ON</button></a>&nbsp;<a href='\/rele?RELE1=0'><button>OFF</button></a></p>\
        <p>R2 <a href='\/rele?RELE2=1'><button>ON</button></a>&nbsp;<a href='\/rele?RELE2=0'><button>OFF</button></a></p>\
        <CENTER>\
        </body>\
        </html>",rel1,rel2);        
        tool->server->send(200, "text/html", tmp);
   
}
void pippo()
{
   tool->GetTelegramBot()->getUpdates(tool->GetTelegramBot()->message[0][1]); 
  for (int i = 1; i < tool->GetTelegramBot()->message[0][0].toInt() + 1; i++)      {
    tool->GetTelegramBot()->sendMessage(tool->GetTelegramBot()->message[i][4], tool->GetTelegramBot()->message[i][5], "");
  }
  tool->GetTelegramBot()->message[0][0] = "";   // All messages have been replied - reset new messages
}

#define TOKEN "161837652:AAHf_tuEv7BbxMjRgYuJZNepYzcjRe1qcvA"
#define NAME "amicodelgiaguaro"
#define USERNAME "cinsky"


void setup(void) {

  //pinMode(RELE1, OUTPUT);
  //pinMode(RELE2, OUTPUT);
  tool = new WiFiTool();
  myserver = tool->server;
  tool->SetTelegramBOT(TOKEN,NAME,USERNAME,(void*)pippo);
  //tool->AddWebPage((void*)Data,(char*)"/data"); 
  //tool->AddWebPage((void*)Rele,(char*)"/rele"); 
}

void loop() {
  tool->HandlServerEvent();   
}
