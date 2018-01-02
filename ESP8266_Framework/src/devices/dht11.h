// 
//    FILE: dht11.h
// VERSION: 0.4.1
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
//     URL: http://playground.arduino.cc/Main/DHT11Lib
//
// HISTORY:
// George Hadjikyriacou - Original version
// see dht.cpp file
// 

#ifndef dht11_h
#define dht11_h

#ifndef DTH11_TEMP_SENSOR
	#define DTH11_TEMP_SENSOR DISABLE
#endif

#if DTH11_TEMP_SENSOR == ENABLE
#define DTH11_SENSOR 0x01
#define DTH11_PORT	 D4



/*
#ifndef _HWD_DRIVER_TMP
	#define _HWD_DRIVER_TMP \
		_HWD_DRIVER_
#endif

#define DHT11_MACRO if(dth11 != NULL){dth11->read(DTH11_PORT);}

#ifdef _HWD_DRIVER_
	#undef _HWD_DRIVER_
#endif

#define _HWD_DRIVER_ \
	_HWD_DRIVER_TMP DHT11_MACRO

*/


#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2

class DTH11
{
	private:
		int humidity;
		int temperature;
	public:
		DTH11(){}
		
		int read(int pin)
		{
			// BUFFER TO RECEIVE
			uint8_t bits[5];
			uint8_t cnt = 7;
			uint8_t idx = 0;

			// EMPTY BUFFER
			for (int i=0; i< 5; i++) bits[i] = 0;

			// REQUEST SAMPLE
			pinMode(pin, OUTPUT);
			digitalWrite(pin, LOW);
			delay(18);
			digitalWrite(pin, HIGH);
			delayMicroseconds(40);
			pinMode(pin, INPUT);

			// ACKNOWLEDGE or TIMEOUT
			unsigned int loopCnt = 10000;
			while(digitalRead(pin) == LOW)
					if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

			loopCnt = 10000;
			while(digitalRead(pin) == HIGH)
					if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

			// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
			for (int i=0; i<40; i++)
			{
					loopCnt = 10000;
					while(digitalRead(pin) == LOW)
							if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

					unsigned long t = micros();

					loopCnt = 10000;
					while(digitalRead(pin) == HIGH)
							if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

					if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
					if (cnt == 0)   // next byte?
					{
							cnt = 7;    // restart at MSB
							idx++;      // next byte!
					}
					else cnt--;
			}

			humidity    = bits[0]; 
			temperature = bits[2]; 
			
			uint8_t sum = bits[0] + bits[2];  

			if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
			return DHTLIB_OK;
		};    
		void getData(void* data){			
			int _out[2];                   // array for two temp bytes
			
			_out[0] = this->humidity;
			_out[1] = this->temperature;	
			memcpy(data,&_out,sizeof(int)*2);
		};
       
};
DTH11 *dth11=new DTH11();
#endif //#if DISPLAY_HT1632 == ENABLE
#endif
//
// END OF FILE
