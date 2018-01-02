#ifndef LM75A_h
#define LM75A_h

#ifndef LM75A_TEMP_SENSOR
	#define LM75A_TEMP_SENSOR DISABLE
#endif

#if LM75A_TEMP_SENSOR == ENABLE 
	#include <Wire.h>
	#define LM75_CHIP	0x00
	#define LM75_IO_ADDR 0x48 
	#define _LM75A_DEBUG_ 
	#define SDA		2 
	#define SCL		0 
	
	class LM75A {
	  
	  private:
		int _address=0;
	  public:
	  
		LM75A(int address):LM75A(){
			this->_address = address; 
		}
		LM75A(){
			#ifdef _LM75A_DEBUG_
				Serial.begin(_BAUD_);   
				Serial.println("begin LM75A");  
			#endif	
			if (_address == 0)
			{
				this->_address = LM75_IO_ADDR;
			}                               		
			Wire.begin(SDA, SCL); // SDA,SCL initialize the I2C/TWI interface 
			
		}
		void  getData(void* data){			
			
			int c[2],                    // array for two temp bytes
			x=1;                     // counter for array (msb is send first)
			long bit_check;
			

			Wire.requestFrom(_address, 2);

			// Get the two bytes we asked for
			while (Wire.available()){
				c[x] = Wire.read();
				x--;
			}
			c[0] = c[0] & 224; // "forget" all bits except the 3 left bits
			c[0] = c[0] >> 5; // shift these 5 bits for the right value

			bit_check = c[1] & 256; // logical and for checking bit 8 if its 1 or 0

			long _out;
			if (bit_check) {
				// 2's complement for negative values
				
				_out =((~c[1]+1)*8 + ~c[0]+1) * 0.125;  
			}
			else {
				_out = (c[1]*8 + c[0]) * 0.125; 
			}			
			memcpy(data,&_out,sizeof(long);
		}
	};
	LM75A *lm75=NULL;
#endif //#if LM75A_TEMP_SENSOR == ENABLE 
#endif
