#include "i2c.h"
#include <delays.h>
#include <timers.h>
#include <stdio.h>
#include <sw_i2c.h>
#define DAC_ADDR 0x27
#define I2C_IO_V4 //setta I2C sui pin trisb6 trisb4

//videolezione a 53:00 circa
//1:08:00 circa per le funzioni
 //OpenI2C(MASTER,SLEW_ON)

//settare SSPADD secondo la formula
//400khz = Fosc/((SSPADD+1)*4)

//(sspadd+1)*400=12000 -> SSPADD+1=30 ->SSPADD=29

main(){
	char temperatureH;
	char temperatureL;
	char humidityH;
	char humidityL;

	int address = DAC_ADDR<<1;
	OpenI2C( MASTER, SLEW_ON );
	address += 1;
	StartI2C();
	WriteI2C(address);
	IdleI2C();
	while(!DataRdyI2C());
	humidityH = ReadI2C();
	//fare il controllo sui byte di stato;
	AckI2C();
	while(!DataRdyI2C());
	humidityL = ReadI2C();
	AckI2C();
	while(!DataRdyI2C());
	temperatureH = ReadI2C();
	AckI2C();
	while(!DataRdyI2C());
	temperatureL = ReadI2C();
	AckI2C();
	NotAckI2C();
	StopI2C();
}