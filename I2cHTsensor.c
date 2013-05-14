#include "i2c.h"
#include "i2cHTsensor.h"
#include "./USB/usb.h"
#include "./USB/usb_function_hid.h"
//#include "keyboard.h"				//include for ASCII codes layout (can be useful)
#include "HardwareProfile.h"

//videolezione a 53:00 circa
//1:08:00 circa per le funzioni
 //OpenI2C(MASTER,SLEW_ON)

int ReadSensor(WORD * HUM, WORD * TEMP){
	WORD T,H;
	char temperatureH;
	char temperatureL;
	char humidityH;
	char humidityL;
	int ERRflag=0;
	char result;
	char address = DAC_ADDR<<1;

	address += 1; //Imposta l'indirizzo del sensore in lettura
	mLED_1_On();
	StartI2C();
	result = WriteI2C(address);
	//mLED_1_Off();
	if(result<0) return -1;
	while(!DataRdyI2C());//finché non ci sono dati aspetta
	humidityH = ReadI2C();//legge i byte alti dell'umidità su cui ci sono 2 bit di stato
	//mLED_1_Off();
	if((humidityH&0b11000000)!=0b0000000) ERRflag=1; //se i bit non danno misura corretta ricorda errore;
	humidityH &=0b00111111; //elimina i byte di stato
	mLED_1_Off();
	AckI2C(); //invia corretta ricezione
	while(!DataRdyI2C());
	humidityL = ReadI2C();
	mLED_1_Off();
	AckI2C();//stesso per i byte bassi
	while(!DataRdyI2C());
	temperatureH = ReadI2C();
	mLED_1_Off();
	AckI2C();
	while(!DataRdyI2C());
	temperatureL = ReadI2C();
	mLED_1_Off();
	temperatureL &= 0b11111100;//protocollo simile per la temperatura ma con diversa maschera per la temperatura (vedi datasheet
	AckI2C();
	NotAckI2C();
	StopI2C();//chiude la prima trasmissione
	if(ERRflag==1) return -1;
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