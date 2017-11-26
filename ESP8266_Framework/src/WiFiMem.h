#ifndef _WIFIMEM_
#define _WIFIMEM_

#include <defs.h>
#include <EEPROM.h>

class WiFiMEM{
	private:	
		int CRC_Error = 0;
		
		struct mem_data
		{
			 
			byte CLIENT_SSID[32];
			byte CLIENT_PWD[32];
			byte SERVER_SSID[32];
			byte SERVER_PWD[32];
			byte SERVER_PORT[2]; 
			byte NTP_SERVER[64];
			byte NTP_ON;
			byte NTP_TYME_ZONE;
			byte COM_BAUD[4]; 
			byte CRC8;
		} eeprom_;		
			
		void loadConfig()    //EA
		{
			uint8_t s;
			int szE = sizeof(eeprom_);
			EEPROM.begin(szE);
			for ( s=0; s<szE; s++) {		
				 EEPROM.get(s,*((byte*)(&eeprom_) + s));
			}			
			EEPROM.commit();
			EEPROM.end();
			unsigned char _crc_mem_ = (unsigned char)this->eeprom_.CRC8;
			unsigned char _crc_calc_ = (unsigned char)CRC8((const byte*)(&eeprom_),szE-1);
			if (_crc_calc_ != _crc_mem_ || (_crc_calc_ + _crc_mem_) == 0 )
			{
				this->CRC_Error = 1;
				memset(this->eeprom_.CLIENT_PWD,0,sizeof(this->eeprom_.CLIENT_PWD));
				memset(this->eeprom_.CLIENT_SSID,0,sizeof(this->eeprom_.CLIENT_SSID));
				memset(this->eeprom_.SERVER_SSID,0,sizeof(this->eeprom_.SERVER_SSID));
				memset(this->eeprom_.SERVER_PWD,0,sizeof(this->eeprom_.SERVER_PWD));
				memset(this->eeprom_.SERVER_PORT,0,sizeof(this->eeprom_.SERVER_PORT));
				//this->eeprom_.SERVER_PORT = 0;
				memset(this->eeprom_.NTP_SERVER,0,sizeof(this->eeprom_.NTP_SERVER));
				this->eeprom_.NTP_TYME_ZONE = 0;
				this->eeprom_.NTP_ON = 0;
				memset(this->eeprom_.COM_BAUD,0,sizeof(this->eeprom_.COM_BAUD));
				//this->eeprom_.COM_BAUD = 0;
			}
#ifdef _WIFIMEM_DEBUG_
			Serial.printf("CRC_MEM:%u\nCRC_CALC:%u\n",_crc_mem_,_crc_calc_);  
#endif
		}
		
		void StoreData()
		{			
			ClearMem();
			uint8_t s;					
			int szE = sizeof(eeprom_);	
			byte _crc_mem_ = CRC8((const byte*)(&eeprom_),szE-1);
			this->eeprom_.CRC8 = _crc_mem_;  			
			EEPROM.begin(szE);			
			for ( s=0; s<szE; s++) {		
				 EEPROM.put(s,*((byte*)(&eeprom_) + s));
				 delay(5);
			}			
			EEPROM.commit();
			EEPROM.end();	
		}
		
		byte CRC8(const byte *data, int len) {
			byte crc = 0x00;
			while (len--) {
				byte extract = *data++;
				for (byte tempI = 8; tempI; tempI--) {
					byte sum = (crc ^ extract) & 0x01;
					crc >>= 1;
					if (sum) {
						crc ^= 0x8C;
					}
					extract >>= 1;
				}
			}
			return crc;
		}
	public:
		WiFiMEM(){
			loadConfig();
		}
		int GetCRCError()
		{
			return this->CRC_Error;
		}
		char *GetServerPWD()
		{
			return (char*)this->eeprom_.SERVER_PWD;
		}
		char *GetServerSSID()
		{
			return (char*)this->eeprom_.SERVER_SSID;
		}
		uint16_t GetServerPort()
		{
			uint16_t out = (this->eeprom_.SERVER_PORT[1] << 8) + 
							this->eeprom_.SERVER_PORT[0];
			return out;
		}
		uint32_t GetCommBaud()
		{
			uint32_t out = (this->eeprom_.COM_BAUD[3] << 24) + 
							(this->eeprom_.COM_BAUD[2] << 16) + 
							(this->eeprom_.COM_BAUD[1] << 8) + 
							this->eeprom_.COM_BAUD[0];
			return out;
			
			//return (uint32_t)this->eeprom_.COM_BAUD;
		}
		char *GetClientPWD()
		{
			return (char*)this->eeprom_.CLIENT_PWD;
		}
		char *GetClientSSID()
		{
			return (char*)this->eeprom_.CLIENT_SSID;
		}

		char *GetNTP()
		{
			return (char*)this->eeprom_.NTP_SERVER;
		}
		int8_t GetNTPTymeZone()
		{
			return (int8_t)this->eeprom_.NTP_TYME_ZONE;
		}
		uint8_t GetNTP_ON()
		{
			return (uint8_t)this->eeprom_.NTP_ON;
		}
		void SetServerPWD(char* pwd)
		{	
			int l = strlen(pwd);
			memset((void*)this->eeprom_.SERVER_PWD,0,32); 	
			memcpy((void*)this->eeprom_.SERVER_PWD,pwd,l);				
		}
		
		void SetServerSSID(char* ssid)
		{
			int l = strlen(ssid);	
			memset((void*)this->eeprom_.SERVER_SSID,0,32); 			
			memcpy((void*)this->eeprom_.SERVER_SSID,ssid,l);						
		}
		void SetClientPWD(char* pwd)
		{
			int l = strlen(pwd);
			memset((void*)this->eeprom_.CLIENT_PWD,0,32); 	
			memcpy((void*)this->eeprom_.CLIENT_PWD,pwd,l);				
		}
		
		void SetClientSSID(char* ssid)
		{	
			int l = strlen(ssid);	
			memset((void*)this->eeprom_.CLIENT_SSID,0,32); 			
			memcpy((void*)this->eeprom_.CLIENT_SSID,ssid,l);						
		}
		void SetNTP(char* ntp)
		{
			int l = strlen(ntp);	
			memset((void*)this->eeprom_.NTP_SERVER,0,64); 			
			memcpy((void*)this->eeprom_.NTP_SERVER,ntp,l);						
		}
		void SetNTP_ON(uint8_t ntp_on)
		{									
			//memcpy((void*)this->eeprom_.NTP_ON,(void*)ntp_on,1);
			this->eeprom_.NTP_ON =(byte)ntp_on;	
		}
		void SetNTP_Tyme_Zone(int8_t ntp_tz)
		{									
			this->eeprom_.NTP_TYME_ZONE = (byte)ntp_tz;						
		}
		void SetServerPort(uint16_t server_port)
		{	
			byte val[2] = { server_port & 0xff, server_port >> 8 };
			memcpy((void*)this->eeprom_.SERVER_PORT,(void*)&val[0], 2 );
		}
		void SetCommBaud(uint32_t com_baud)
		{	
			byte val[4] = { com_baud & 0xff, com_baud >> 8,com_baud >> 16,com_baud >> 24 };								
			//this->eeprom_.COM_BAUD=
			memcpy((void*)this->eeprom_.COM_BAUD,(void*)&val[0], 4 );
			//this->eeprom_.COM_BAUD = com_baud;
		}

		void Save()
		{
			StoreData();			
		}
		void Load()
		{
			loadConfig();			
		}
		void ClearMem()
		{
			int szE = sizeof(this->eeprom_);			
			EEPROM.begin(szE);
			for(int i=0;i<szE;i++){									
				EEPROM.put(i,0);						
			}					
			EEPROM.commit();
			EEPROM.end();
		}
		
};
#endif
