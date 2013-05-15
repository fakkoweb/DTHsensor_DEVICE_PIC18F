#include "i2c.h"
#include "i2cHTsensor.h"
#include "./USB/usb.h"
#include "./USB/usb_function_hid.h"
//#include "keyboard.h"				//include for ASCII codes layout (can be useful)
#include "HardwareProfile.h"

//videolezione a 53:00 circa
//1:08:00 circa per le funzioni
 //OpenI2C(MASTER,SLEW_ON)

int RequestMeasureTH(){
	unsigned char address = DAC_ADDR<<1;
	int result;
	StartI2C();
	result=WriteI2C(address);
	StopI2C();

	if(result==-1){
	return 0;
 	}//busy;
	if(result==-2){
	mLED_1_Off();
	mLED_1_On();
	 //notAck;
	return 1;
	}
	//if(resutl==0) //ok;
	return 1;
}


int ReadSensor(WORD * HUM, WORD * TEMP){
	WORD T,H;
	unsigned char temperatureH;
	unsigned char temperatureL;
	unsigned char humidityH;
	unsigned char humidityL;
	unsigned char maschera;
	unsigned int STALEflag=0;
	unsigned char result;
	unsigned char address = DAC_ADDR<<1;

	address |= 0b00000001; //Imposta l'indirizzo del sensore in lettura
	StartI2C();
	result = WriteI2C(address);
	humidityH = ReadI2C();//legge i byte alti dell'umidità su cui ci sono 2 bit di stato
	if(humidityH & 0b11000000) STALEflag=1; //se i bit non danno misura corretta ricorda errore;
	humidityH &=0b00111111; //elimina i byte di stato
	AckI2C(); //invia corretta ricezione
	humidityL = ReadI2C();
	AckI2C();//stesso per i byte bassi
	temperatureH = ReadI2C();
	AckI2C();
	temperatureL = ReadI2C();
	temperatureL &= 0b11111100;//protocollo simile per la temperatura ma con diversa maschera per la temperatura (vedi datasheet
	NotAckI2C();
	StopI2C();//chiude la prima trasmissione
	if(STALEflag==1) return -1;
	//elaborazione dei dati in 2 word;
	T=0;
	H=0;
	T = T + (WORD)temperatureH;
	T = T<<8;
	T = T + (WORD)temperatureL;
	T= T>>2;
	H = H + (WORD)humidityH;
	H = H<<8;
	H = H + (WORD)humidityL;
	*TEMP=T;
	*HUM=H;
	return 1;
}