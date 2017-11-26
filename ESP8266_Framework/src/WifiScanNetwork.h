#ifndef _WIFI_SCAN_NETWORK_
#define _WIFI_SCAN_NETWORK_


class ESP8266_ScanNetwork{
	private:
		
	protected:
	public:
	
		ESP8266_ScanNetwork(){
			// Set WiFi to station mode and disconnect from an AP if it was previously connected
			WiFi.mode(WIFI_STA);
			WiFi.disconnect();
			//delay(500);
		}
		String EnumNetwork()
		{
			String out = "";
			
			// WiFi.scanNetworks will return the number of networks found
			int n = WiFi.scanNetworks();
			if (n == 0){
				out = "'" + String(NO_NETWORKS) + "'";
			}
			else
			{	
				out="";
				for (int i = 0; i < n-1; ++i)
				{
					out+="'" + WiFi.SSID(i) + "',";					
					delay(20);
				}
				out+="'" + WiFi.SSID(n-1) + "'";					
			}
			return out;
		
		}						
};
#endif
