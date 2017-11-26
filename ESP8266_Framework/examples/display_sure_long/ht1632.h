#ifndef _HT1632_
  #define _HT1632_
/*
 * ht1632.h
 * defintions for Holtek ht1632 LED driver.
 */


/*
 * commands written to the chip consist of a 3 bit "ID", followed by
 * either 9 bits of "Command code" or 7 bits of address + 4 bits of data.
 */
 
 
#define HT1632_ID_CMD 4		// ID = 100 - Commands 
#define HT1632_ID_RD  6		// ID = 110 - Read RAM 
#define HT1632_ID_WR  5		// ID = 101 - Write RAM 

#define HT1632_ID_BITS (1<<2)   /* IDs are 3 bits */

#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSON  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-00xx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x14	/* CMD= 0001-01xx-x Master Mode */
#define HT1632_CMD_RCCLK  0x18	/* CMD= 0001-10xx-x Use on-chip clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM    0xA0	/* CMD= 101x-PPPP-x PWM duty cycle */
#define HT1632_CMD_BITS (1<<7)
#define NUM_CHANNEL 2


#define plot(x,y,v)  ht1632_plot(x,y,v)
#define cls          ht1632_clear
// output pins for display;

#define HT1632_DATA     4    // Data pin (pin 7)
#define HT1632_WRCLK    5    // Write clock pin (pin 5)
#define HT1632_CS       16    // Chip Select (1, 2, 3, or 4)


#ifdef _16x24_
  #define X_MAX 23
  #define Y_MAX 15
#else
  #define X_MAX 31
  #define Y_MAX 7
#endif

class HT1632 {
  private: 
    /*void ht1632_plot(int, int, char );
    void ht1632_chipselect(byte);
    void ht1632_chipfree(byte );
    void ht1632_writebits (byte, byte );
    static void ht1632_sendcmd (byte d);
    void ht1632_clear();
    static void ht1632_senddata (byte , byte );
    void ht1632_putchar(int , int , char );
    void ht1632_setup();
    void ht1632_putTinyChar(int , int , char );
    void displayScrollingLine(char* , int);
    void displayScrollingLine();
    byte get_shadowram(byte , byte );
    void snapshot_shadowram();
    void ht1632_putSprite(int , int , char*);
    */
    // current line string is built by copying each character to the position of the pointer, then advancing the pointer;
    char  msgLine[180] = {0};
    char* msgLinePtr = &msgLine[0];
    int   msgLineSize  = 0;
    int   msgLineIndex = 0;




   //*********************************************************************************************************
  /*
   * we keep a copy of the display controller contents so that we can
   * know which bits are on without having to (slowly) read the device.
   * Note that we only use the low four bits of the shadow ram, since
   * we're shadowing 4-bit memory.  This makes things faster, and we
   * use the other half for a "snapshot" when we want to plot new data
   * based on older data...
   */
   // (fc) covers the case for both 16x24 (96 bytes, 4 bits) and 32x8 (64 bytes, 4 bits);
   byte ht1632_shadowram[96];  // our copy of the display's RAM


    //*********************************************************************************************************
    /*
     * plot a point on the display, with the upper left hand corner
     * being (0,0), and the lower right hand corner being (23, 15).
     * Note that Y increases going "downward" in contrast with most
     * mathematical coordiate systems, but in common with many displays
     * No error checking; bad things may happen if arguments are out of
     * bounds!  (The ASSERTS compile to nothing by default
     */
    void ht1632_plot(int x, int y, char val)
    {
      if (x<0 || x>X_MAX || y<0 || y>Y_MAX)
         return;
    
      char addr, bitval;
    
      /*
       * The 4 bits in a single memory word go DOWN, with the LSB
       * (first transmitted) bit being on top.  However, writebits()
       * sends the MSB first, so we have to do a sort of bit-reversal
       * somewhere.  Here, this is done by shifting the single bit in
       * the opposite direction from what you might expect.
       */
      bitval = 8>>(y&3);       // compute which bit will need set
    
      // compute which memory byte this is in;
    #ifdef _16x24_
      addr = (x<<2) + (y>>2);
    #else
    // (fc)
      addr = (x<<1) + (y>>2);
    #endif
    
      if (val) {  // Modify the shadow memory
        ht1632_shadowram[addr] |= bitval;
      }
      else {
        ht1632_shadowram[addr] &= ~bitval;
      }
    
      // Now copy the new memory value to the display
      ht1632_senddata(addr, ht1632_shadowram[addr]);
    }

    /***********************************************************************
     * ht1632_chipselect / ht1632_chipfree
     * Select or de-select a particular ht1632 chip.
     * De-selecting a chip ends the commands being sent to a chip.
     * CD pins are active-low; writing 0 to the pin selects the chip.
     ***********************************************************************/
    
    void ht1632_chipselect(byte chipno)
    {
      digitalWrite(chipno, 0);
    }

    void selectChannel(uint8_t channel) {
    	//if(channel < NUM_CHANNEL) {
    	//	_tgtChannel = channel;
    	//}
    }

    void ht1632_chipfree(byte chipno)
    {
      digitalWrite(chipno, 1);
    }

    //*********************************************************************************************************
    /*
     * ht1632_writebits
     * Chip is assumed to already be chip-selected
     * Bits are shifted out from MSB to LSB, with the first bit sent
     * being (bits & firstbit), shifted till firsbit is zero.
     */
    void ht1632_writebits (byte bits, byte firstbit)
    {
      while (firstbit)
      {
        digitalWrite(HT1632_WRCLK, LOW);
        if (bits & firstbit)
        {
          digitalWrite(HT1632_DATA, HIGH);
        }
        else
        {
          digitalWrite(HT1632_DATA, LOW);
        }
        digitalWrite(HT1632_WRCLK, HIGH);
        firstbit >>= 1;
      }
    }


    //*********************************************************************************************************
    /*
     * ht1632_sendcmd
     * Send a command to the ht1632 chip.
     * A command consists of a 3-bit "CMD" ID, an 8bit command, and
     * one "don't care bit".
     *   Select 1 0 0 c7 c6 c5 c4 c3 c2 c1 c0 xx Free
     */
    void ht1632_sendcmd (byte command)
    {
      ht1632_chipselect(HT1632_CS);          // Select chip
      ht1632_writebits(HT1632_ID_CMD, 1<<2); // send 3 bits of id: COMMMAND
      ht1632_writebits(command, 1<<7);       // send the actual command
      ht1632_writebits(0, 1);                // one extra dont-care bit in commands
      ht1632_chipfree(HT1632_CS);            // done
    }


    //*********************************************************************************************************
    /*
     * ht1632_clear
     * clear the display, and the shadow memory, and the snapshot
     * memory.  This uses the "write multiple words" capability of
     * the chipset by writing all 96 words of memory without raising
     * the chipselect signal.
     */
    void ht1632_clear()
    {
      char i;
    
      ht1632_chipselect(HT1632_CS);          // Select chip
      ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
      ht1632_writebits(0, 1<<6);             // Send address
      for (i = 0; i < 96/2; i++)             // Clear entire display
        ht1632_writebits(0, 1<<7);           // send 8 bits of data
      ht1632_chipfree(HT1632_CS);            // done
    
      for (i=0; i<96; i++)
        ht1632_shadowram[i] = 0;
    }
    
    void ht1632_cls()
    {
      char i;
    
      ht1632_chipselect(HT1632_CS);          // Select chip
      ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
      ht1632_writebits(0, 1<<6);             // Send address
      for (i = 0; i < 96/2; i++)             // Clear entire display
        ht1632_writebits(0, 1<<7);           // send 8 bits of data
      ht1632_chipfree(HT1632_CS);            // done    
    }

    //*********************************************************************************************************
    /*
     * ht1632_senddata
     * send a nibble (4 bits) of data to a particular memory location of the
     * ht1632.  The command has 3 bit ID, 7 bits of address, and 4 bits of data.
     *    Select 1 0 1 A6 A5 A4 A3 A2 A1 A0 D0 D1 D2 D3 Free
     * Note that the address is sent MSB first, while the data is sent LSB first!
     * This means that somewhere a bit reversal will have to be done to get
     * zero-based addressing of words and dots within words.
     */
    void ht1632_senddata (byte address, byte data)
    {
      ht1632_chipselect(HT1632_CS);          // Select chip
      ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
      ht1632_writebits(address, 1<<6);       // Send address
      ht1632_writebits(data, 1<<3);          // send 4 bits of data
      ht1632_chipfree(HT1632_CS);            // done
    }
    

    //*********************************************************************************************************
    void ht1632_setup()
    {
      pinMode(HT1632_CS, OUTPUT);
      digitalWrite(HT1632_CS, HIGH);      // unselect (active low)
      pinMode(HT1632_WRCLK, OUTPUT);
      pinMode(HT1632_DATA, OUTPUT);
      ht1632_sendcmd(HT1632_CMD_SYSDIS);  // Disable system
    
    #ifdef _16x24_
      ht1632_sendcmd(HT1632_CMD_COMS11);  // 16*32, PMOS drivers
    #else
    // (fc)
      ht1632_sendcmd(HT1632_CMD_COMS10);  // 32x8, PMOS drivers
    #endif
    
      ht1632_sendcmd(HT1632_CMD_MSTMD); 	// Master Mode
      ht1632_sendcmd(HT1632_CMD_SYSON); 	// System on
      ht1632_sendcmd(HT1632_CMD_LEDON); 	// LEDs on
    
      for (byte i=0; i<96; i++)
        ht1632_senddata(i, 0);  // clear the display!
    
      delay(100);  // REM:
    }


    //*********************************************************************************************************
    /*
     * Copy a character glyph from the myfont data structure to
     * display memory, with its upper left at the given coordinate
     * This is unoptimized and simply uses plot() to draw each dot.
     */
    void ht1632_putchar(int x, int y, char c)
    {
      // fonts defined for ascii 32 and beyond (index 0 in font array is ascii 32);
      byte charIndex;
    
      // replace undisplayable characters with blank;
      if (c < 32 || c > 126)
      {
        charIndex = 0;
      }
      else
      {
        charIndex = c - 32;
      }
    
      // move character definition, pixel by pixel, onto the display;
      // fonts are defined as one byte per row;
      for (byte row=0; row<8; row++)
      {
        byte rowDots = pgm_read_byte_near(&myfont[charIndex][row]);
        for (byte col=0; col<6; col++)
        {
          if (rowDots & (1<<(5-col)))
            plot(x+col, y+row, 1);
          else
            plot(x+col, y+row, 0);
        }
      }
    }



    //*********************************************************************************************************
    /*void ht1632_putTinyChar(int x, int y, char c)
    {
    	// fonts defined for ascii 32 and beyond (index 0 in font array is ascii 32);
    	byte charIndex;
    
    	// replace undisplayable characters with blank;
    	if (c < 32 || c > 127)
    	{
    		charIndex	=	0;
    	}
    	else
    	{
    		charIndex	=	c - 32;
    	}
    
    	// move character definition, pixel by pixel, onto the display;
    	// fonts are defined as one byte per row;
    	for (byte row=0; row<8; row++)
    	{
    		byte rowDots	=	pgm_read_byte_near(&tinyFont[charIndex][row]);
    		for (byte col=0; col<4; col++)
    		{
    			if (rowDots & (1<<(3-col)))
    				ht1632_plot(x+col, y+row, 1);
    			else 
    				ht1632_plot(x+col, y+row, 0);
    		}
    	}
    }*/
    



    
    
    

    //*********************************************************************************************************
    /*
     * get_shadowram
     * return the value of a pixel from the shadow ram.
     */
    byte get_shadowram(byte x, byte y)
    {
      byte addr, bitval;
    
      bitval = 8>>(y&3);       // compute which bit will need set
      addr = (x<<2) + (y>>2);  // compute which memory word this is in
    
      return (0 != (ht1632_shadowram[addr] & bitval));
    }
    
    
    //*********************************************************************************************************
    /*
     * snapshot_shadowram
     * Copy the shadow ram into the snapshot ram (the upper bits)
     * This gives us a separate copy so we can plot new data while
     * still having a copy of the old data.  snapshotram is NOT
     * updated by the plot functions (except "clear")
     */
    void snapshot_shadowram()
    {
      for (char i=0; i< sizeof(ht1632_shadowram); i++)
      {
        ht1632_shadowram[i] = (ht1632_shadowram[i] & 0x0F) | ht1632_shadowram[i] << 4;  // Use the upper bits
      }
    }
    
    
    //*********************************************************************************************************
    /*
     * get_snapshotram
     * get a pixel value from the snapshot ram (instead of
     * the actual displayed (shadow) memory
     */
    byte get_snapshotram(byte x, byte y)
    {
      byte addr, bitval;
    
      bitval = 128>>(y&3);  // user upper bits!
    
      // compute which memory byte this is in;
    #ifdef _16x24_
      addr = (x<<2) + (y>>2);
    #else
    // (fc)
      addr = (x<<1) + (y>>2);
    #endif
    
      if (ht1632_shadowram[addr] & bitval)
        return 1;
    
      return 0;
    }
    
    
    //*********************************************************************************************************
    void ht1632_putSprite(int x, int y, char* sprite)
    {
      // move sprite, pixel by pixel, onto the display;
      for (byte row=0; row<8; row++)
      {
        byte rowDots = sprite[row];
        for (byte col=0; col<8; col++)
        {
          if (rowDots & (1<<(7-col)))
            plot(x+col, y+row, 1);
          else
            plot(x+col, y+row, 0);
        }
      }
    }
    
  public:
    HT1632()
    {
      ht1632_setup();
      ht1632_clear();
      delay(1000);
    }

      
    // Brighness is from 0 to 15
    void setBrightness( unsigned char brightness ) {      
        ht1632_sendcmd(HT1632_CMD_PWM | (brightness & 0x0F ));      
    }
    
    //*********************************************************************************************************
    /*
    * This works equally well for both 16x24 and 8x32 matrices.
    */
    
    void displayScrollingLine(char* msg, int crtPos)
    {
       ht1632_sendcmd(HT1632_CMD_LEDON);
      // shift the whole screen 6 times, one column at a time;
      int stLen = strlen(msg);
        
      char spaceStr[10 + stLen + 1];
      sprintf(spaceStr, "     %s     " ,msg);
      stLen += 6;  
         
      for (int x=0; x<(stLen*6); x++)
      {                    
          //ht1632_putchar(-x, 0, msg[crtPos]);
          ht1632_putchar(-x, 0, spaceStr[crtPos]);
                
          for(int k=1; k<(stLen); k++)
          {
              ht1632_putchar(-x+(k*6), 0, ((crtPos+k < stLen + 5) ? spaceStr[crtPos+k] : ' '));       
              
              delay(4);
          }                 
      }
      
    }

    void displayCenterLine(char* msg, int crtPos)
    {
       ht1632_sendcmd(HT1632_CMD_LEDON);
      // shift the whole screen 6 times, one column at a time;
      int stLen = strlen(msg);
        
      char spaceStr[5 + stLen + 1];
      sprintf(spaceStr, "     %s" ,msg);
      stLen += 6;
         
      for (int x=0; x<(stLen*6); x++)
      {                    
          ht1632_putchar(-x, 0, spaceStr[crtPos]);
                
          for(int k=1; k<(stLen+20); k++)
          {
              ht1632_putchar(-x+(k*6), 0, ((crtPos+k < stLen + 5) ? spaceStr[crtPos+k] : ' '));       
              
              delay(1);
          }                 
      }      
    }
    
    void displayClock(char* msg, int crtPos)
    {
       ht1632_sendcmd(HT1632_CMD_LEDON);
      // shift the whole screen 6 times, one column at a time;
      int stLen = strlen(msg);
        
      //char spaceStr[5 + stLen + 1];
      //sprintf(spaceStr, "     %s" ,msg);
      stLen += 6;
     for(int k=0; k<(stLen); k++)
     {
              ht1632_putchar((k*6), 0, msg[k]);       
              
              delay(1);
      }                      
      
    }
    void fadeUp()
    { 
      ht1632_sendcmd(HT1632_CMD_LEDON);
      setBrightness(0);
      for(int b=0; b<16; b++)
      {
        setBrightness((unsigned char)b);
        delay(20);
      }
    }
    void fadeDown()
    {
      
       for(int b1=15; b1>0; b1--)
      {
        setBrightness((unsigned char)b1);
        delay(20);
      }
      setBrightness(0);
      ht1632_sendcmd(HT1632_CMD_LEDOFF);
      //ht1632_cls();
    }
    void displayEffect(char* msg, int crtPos)
    {
      // shift the whole screen 6 times, one column at a time;
      int stLen = strlen(msg);
        
      //char spaceStr[5 + stLen + 1];
      //sprintf(spaceStr, "     %s" ,msg);
      stLen += 6;
      
      setBrightness(0);


      
      for(int k=0; k<(stLen); k++)
      {
        ht1632_putchar((k*6), 0, msg[k]);       
              
        delay(1);
      }                 
      fadeUp();
      delay(1000);
      fadeDown();
      delay(1000);
      fadeUp();
      delay(1000);
      fadeDown();
      
    }  
    
};
    
#endif
