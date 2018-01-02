#ifndef _SERVICE_NTP_
#define _SERVICE_NTP_

#include <defs.h>
#include <WiFiUdp.h>
#include <cstring>

extern "C" {
#include "user_interface.h"
}

void NTP_data(void);

/* Don't hardwire the IP address or we won't get the benefits of the pool.
	Lookup the IP address for the host name instead */
	
IPAddress timeServerIP; // time.nist.gov NTP server address
char ntpServerName[256];  //= NTP_SERVER;

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
		
os_timer_t myTimer;
unsigned long secsSince1900;

int hours=0;
int minutes=0;
int seconds=0;

unsigned long sendNTPpacket(IPAddress& address)
{
#ifdef _NTP_DEBUG_
	Serial.println("sending NTP packet...");
#endif
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.beginPacket(address, 123); //NTP requests are to port 123
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}

void localSeconds()
{
	if((seconds==30)||(seconds==0)) {
		
		//get a random server from the pool
		WiFi.hostByName(ntpServerName, timeServerIP); 

		sendNTPpacket(timeServerIP); // send an NTP packet to a time server
		// wait to see if a reply is available
  
		int cb = udp.parsePacket();
		int _exit_Loop_ = 0;
		while(!cb)
		{	
			if(_exit_Loop_ == 10 ){break;}else{delay(25);} 
			_exit_Loop_ ++;
		}
		if (!cb) {
#ifdef _NTP_DEBUG_
			Serial.println("no packet yet");
#endif
		}
		else {
#ifdef _NTP_DEBUG_
			Serial.print("packet received, length=");
			Serial.println(cb);
#endif
			// We've received a packet, read the data from it
			udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

			//the timestamp starts at byte 40 of the received packet and is four bytes,
			// or two words, long. First, esxtract the two words:
			unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			// combine the four bytes (two words) into a long integer
			// this is NTP time (seconds since Jan 1 1900):
			unsigned long secsSince1900 = highWord << 16 | lowWord;
    
#ifdef _NTP_DEBUG_    
			Serial.print("Seconds since Jan 1 1900 = " );
			Serial.println(secsSince1900);
			// now convert NTP time into everyday time:
			Serial.print("Unix time = ");
#endif
			// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
			const unsigned long seventyYears = 2208988800UL;
			// subtract seventy years:
			unsigned long epoch = secsSince1900 - seventyYears;
#ifdef _NTP_DEBUG_    
			// print Unix time:
			Serial.println(epoch);
			// print the hour, minute and second:
			Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
#endif    
			hours = ((epoch  % 86400L) / 3600);
			minutes = ((epoch % 3600) / 60);
			seconds =  (epoch % 60);
#ifdef _NTP_DEBUG_
			Serial.print(hours); // print the hour (86400 equals secs per day)
			Serial.print(':');
			if ( minutes ) {
				// In the first 10 minutes of each hour, we'll want a leading '0'
				Serial.print('0');
			}
			Serial.print(minutes); // print the minute (3600 equals secs per minute)
			Serial.print(':');
			if ( seconds ) {
				// In the first 10 seconds of each minute, we'll want a leading '0'
				Serial.print('0');
			}
			Serial.println(seconds); // print the second
#endif
		}
	}else{
		if(seconds == 59)
		{
			seconds = 0;
		}else{
			seconds ++;
		}
	}
}



class Service_NTP{
	private:
		int _ltz = 0;
	protected:
	public:
		Service_NTP(String ntpserver):Service_NTP()
		{			
			memset(ntpServerName,0,256);
			std::strcpy(ntpServerName,ntpserver.c_str()); 
		}
		Service_NTP(){
				
			
			memset(ntpServerName,0,256);
			memcpy((void*)ntpServerName,(const void*)NTP_SERVER_,strlen((const char*)NTP_SERVER_));
			
#ifdef _NTP_DEBUG_
			Serial.println("begin NTP");  
#endif
			udp.begin(UDP_PORT);
			secsSince1900 = 0;		
			//NTP_data();			
		}
		void SetLocalTimeZone (int ltz)
		{
			this->_ltz = ltz;
		}

		int GetLTZMinutes(){return minutes;}
		int GetLTZSeconds(){return seconds;}
		int GetLTZHours(){
			
			if ((this->_ltz  +  hours) > 23){
				return (hours + this->_ltz) - 24;  
			}
			return (this->_ltz + hours);
		}
		
		int GetUTCMinutes(){return minutes;}
		int GetUTCSeconds(){return seconds;}
		int GetUTCHours(){return hours;}
		
		int SettUTCMinutes(int mins){minutes = mins;}
		int SetUTCSeconds(int secs){seconds = secs;}
		int SetUTCHours(int hrs){hours= hrs;}
		
		String GetNTPServer(){return String(ntpServerName);	}
			
};
#endif
