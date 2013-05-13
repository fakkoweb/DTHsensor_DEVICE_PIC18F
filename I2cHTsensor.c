#include "i2c.h"



//videolezione a 53:00 circa
//1:08:00 circa per le funzioni
 //OpenI2C(MASTER,SLEW_ON)

//settare SSPADD secondo la formula
//400khz = Fosc/((SSPADD+1)*4)

//(sspadd+1)*400=12000 -> SSPADD+1=30 ->SSPADD=29

 //CloseI2C
 //IdleI2C serve in multimaster
 //startI2C genera la condizione di start
 //Restart genera la condizione di restart
 //StopI2c
 //ackI2c
 //notackI2C
 //DataRdyI2C dopo 8 colpi di clock passa info
 //readI2C
 //writeI2c


//start -> scrivere indirzzo spostato di un bit -> vedere se ha risposto ack -> stoppare e cercare se ha mandato i dati


