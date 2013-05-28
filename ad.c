#include "./USB/usb.h"
#include "./USB/usb_function_hid.h"
//#include "keyboard.h"				//include for ASCII codes layout (can be useful)
#include "HardwareProfile.h"


void ADCInit(void)
{
 // TRISC = 0x00;	        // Set PORTC as Output
 // ANSEL = 0;            // Set as Digital I/O
 // ANSELH = 0;           // Set as Digital I/O

  TRISCbits.TRISC7 = 1; // Set RC7  (pin 09, aka AN09) to input
  ANSELHbits.ANS9 = 1; // Set AN09 (pin 09, aka RC7)  to analog
  
  ADCON0=0b00100101;    // Channel AN09 (xx10 01xx), Enable ADC (xxxx xxx1)
  ADCON1=0b00000000;    // VDD and VSS as voltage reference (xxxx 0000)
  ADCON2=0b10001110;    // Right justify result (1xxx xxx), 2 TAD delay (xx00 1xxx), TAD = 1.33 us (Fck/64 = 0.75 Mhz) (xxxx x110) 

}



short int GetDust(void)
{
	BYTE a[2];
	unsigned short int dust;
 	ADCON0bits.GO = 1;              // Start conversion
    //PORTCbits.RC4 = 1;            // RC4 up at start of conversion - Debug and timing
    while (ADCON0bits.GO);          // Wait conversion done
    //PORTCbits.RC4 = 0;            // RC4 down at end of conversion - Debug and timing
	a[1]=ADRESH;
	a[0]=ADRESL;
	memcpy( (void*)&dust, (void*)&a, 2);
    //dust = ( (WORD)ADRESH << 8) + ADRESL; // Read ADC registers and convert to 0-1023 integer             
    //PORTC = (voltage >> 6) & 0x0F;  Dispaly 4 MSB
	return dust;
}
