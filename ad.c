#include "./USB/usb.h"
#include "./USB/usb_function_hid.h"
//#include "keyboard.h"				//include for ASCII codes layout (can be useful)
#include "HardwareProfile.h"
#include "delays.h"

short int GetDust(void)
{
	short int dust;
	TRISBbits.TRISB7 = 0; // Set PORTB as Output
  	ANSEL = 0;    // Set as Digital I/O
  	ANSELH = 0;   // Set as Digital I/O

 	TRISCbits.TRISC7 = 1; // Set RC7  (pin 09, aka AN09) to input
  	ANSELHbits.ANS9 = 1; // Set AN09 (pin 09, aka RC7)  to analog
  
  	ADCON0=0b00100101;    // Channel AN09 (xx10 01xx), Enable ADC (xxxx xxx1)
  	ADCON1=0b00000000;    // VDD and VSS as voltage reference (xxxx 0000)
  	ADCON2=0b10001110;    // Right justify result (1xxx xxx), 2 TAD delay (xx00 1xxx), TAD = 0.17 us (Fck/64 = 0.75 Mhz) (xxxx x110) 

    LATBbits.LATB7 = 0;   //LED on   
	Delay100TCYx(34);	//not sure..1TCY 4/48M???,0,28ms
 	ADCON0bits.GO = 1;  // Start conversion
    while (ADCON0bits.GO);// Wait conversion done
	Delay100TCYx(5); //0,04ms
    LATBbits.LATB7 = 1;   //LED off
	Delay1KTCYx(20);//9,68ms
    dust = ( (short int)ADRESH << 8) + ADRESL; // Read ADC registers and convert to 0-1023 integer             
	return dust;
}
