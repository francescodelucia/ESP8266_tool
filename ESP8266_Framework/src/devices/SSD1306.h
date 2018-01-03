#ifndef _DISPLAY_SSD1306_
	#define _DISPLAY_SSD1306_
	
	#ifndef DISPLAY_SSD1306
		#define DISPLAY_SSD1306 DISABLE
	#endif
	
	#if DISPLAY_SSD1306 == ENABLE
		#include <Wire.h>
		#include <ACROBOTIC_SSD1306.h>
		#define SSD1306_DISPLAY	0x05
		
		
		#define SSD1306_INIT 0x0501
		//void init(void);
		#define SSD1306_SET_NORMAL_DISPLAY 0x0502
		//void setNormalDisplay();
		#define SSD1306_SET_INVERSE_DISPLAY 0x0503
		//void setInverseDisplay();
		#define SSD1306_SEND_CMD 0x0504
		//void sendCommand(unsigned char command);
		#define SSD1306_SEND_DATA 0x0505
		//void sendData(unsigned char Data);
		#define SSD1306_PAGE_MODE 0x0506
		//void setPageMode();
		#define SSD1306_SET_H_MODE 0x0507
		//void setHorizontalMode();
		#define SSD1306_SET_TEXT_XY 0x0508
		//void setTextXY(unsigned char Row, unsigned char Column);
		#define SSD1306_CLEAR 0x0509
		//void clearDisplay();
		#define SSD1306_BRIGHT 0x0510
		//void setBrightness(unsigned char Brightness);
		#define SSD1306_PUT_CHAR 0x0511
		//bool putChar(unsigned char c);
		#define SSD1306_PUT_STR 0x0512
		//void putString(const char *string);		
		//#define SSD1306_PUT_STR2 0x0513
		//void putString(String string);
		#define SSD1306_PUT_NUMBER 0x0514
		//unsigned char putNumber(long n);
		#define SSD1306_PUT_FLOAT 0x0515
		//unsigned char putFloat(float floatNumber,unsigned char decimal);		
		//unsigned char putFloat(float floatNumber);
		#define SSD1306_DRAW_BMP 0x0516
		//void drawBitmap(unsigned char *bitmaparray,int bytes);
		#define SSD1306_SET_H_SCROLL 0x0517
		/*
		void setHorizontalScrollProperties(
        bool direction,
        unsigned char startPage, v
        unsigned char endPage, 
        unsigned char scrollSpeed);*/
        #define SSD1306_SET_SCROLL_ON 0x0518
		//void activateScroll();
		#define SSD1306_SET_SCROLL_OFF 0x0519
		//void deactivateScroll();
		#define SSD1306_SET_FONT 0x0520
		//void setFont(const uint8_t* font);
		
		ACROBOTIC_SSD1306 *SSD1306 = &oled;
	#endif //#if DISPLAY_HT1632 == ENABLE
#endif
