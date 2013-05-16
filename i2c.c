// PIC18F14K50 in C - Versione 0.5 - Novembre 2010
// Copyright (c) 2010 VincenzoV.net
// Creative Commons | Attribuzione-Condividi allo stesso modo 3.0 Unported.
// Creative Commons | Attribution-Share Alike 3.0 Unported
// http://www.vincenzov.net/tutorial/elettronica-di-base/PIC18F14K50

// Switch: I2C master

#define DAC_ADDR 7

#include <p18f14k50.h> 
#include <delays.h>
#include <timers.h>
#include <stdio.h>
#include <i2c.h>
#include <sw_i2c.h>


#pragma config FOSC = IRCCLKOUT  // Internal RC oscillator (F/4 to OSC2 - pin 3) 
#pragma config WDTEN = OFF       // Watchdog disabled
#pragma config LVP = OFF         // Disable Low Voltage Programming
#pragma config MCLRE = OFF       // Disable MCLR

void write_to_DAC (unsigned char chanel, unsigned char value)
{chanel = chanel & 0x03;           // Chanel from 0 to 3
 StartI2C();                       // Start I2C communication
 IdleI2C();
 WriteI2C( 0x50 | (DAC_ADDR<<1) ); // Send device address
 IdleI2C();
 WriteI2C( chanel );               // Send control byte
 IdleI2C();
 WriteI2C( value );                // Send data
 IdleI2C();
 StopI2C();
}

char write_to_DAC_V2 (unsigned char chanel, unsigned char value)
{unsigned char message[4];
 char error_code;
 
 message[0] = 0x50 | (DAC_ADDR<<1); // Device address
 message[1] = chanel & 0x03;        // Control byte
 message[2] = value;                // Data
 message[3] = 0;
 StartI2C(); 
 error_code = putsI2C (message);    // Send all
 StopI2C();
 return (error_code);
}

void write_to_DAC_SW (unsigned char chanel, unsigned char value)
{chanel = chanel & 0x03;            // Chanel from 0 to 3
 SWStartI2C();
 SWPutcI2C( 0x50 | (DAC_ADDR<<1) ); // Send device address
 SWAckI2C();
 SWPutcI2C( chanel );               // Send control byte
 SWAckI2C();
 SWPutcI2C( value );                // Send data
 SWAckI2C();
 SWStopI2C();
}

void main (void)        // Program start here
{unsigned char i; 
 
 OSCCON = 0x70;         // Set internal clock to 16 MHz
 TRISC = 0x00;          // Set PORTC as Output 
 PORTC = 0x00;          // Clear PORTC
 
 ANSEL = 0;             // Set as Digital I/O
 ANSELH = 0;            // Set as Digital I/O

 OpenI2C( MASTER, SLEW_ON ); // I2C Master mode, Slew rate enabled (for 400 kHz)
 SSPADD = 9;                 // Set 400 kHz clock ( @ 16 MHz )
 
 while (1)
   {write_to_DAC_SW (3, i++);
    write_to_DAC_SW (1, 255-i); 
   }

}