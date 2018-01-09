API WifiTool


Main file WiFiTool.h
  
  - WiFiTool(int PersonalWebRoot /* 1 = YES ; 0 = NO */) : 
    WiFiTool constructor, you can use parameter "PersonalWebRoot" for create your personal root page**.
    This method, inizialize the the Wifi AP and client WiFi connection. Authentication, Name AP, and other,
    are stored on EEPROM of device, for Cutomize parameter yoou can connect to WenPage Manager. 
    
    ** you can see base root page on private method itWork().
    
  - AddWebRoot(void* callbackWebRoutine) :
     In case you decide to Customize WebRoot Page, you can add with this method the new routine for generate new WebRoot,
     How you can create, try to see for example  itWork() private method.
     
    
   - AddWebPage(void* callbackWebRoutine,char* webpagename) :
     With this Method you can add your personal page, creating your callback routine page (callbackWebRoutine), and use 
     parameter "webpagename" for identify when this page show.
      
     es.
     
      tool->AddWebPage((void*)Rele,(char*)"/rele");
      /* 
        Routine "Rele" called when URL are http://{yourip}/rele
      */
     Note. See Project on Example source code.
			
			
			
###############################################################################
	
	In Progress editing

###############################################################################
 
    
    

