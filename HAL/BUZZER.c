#include "BUZZER.h"

void buzzerInit(uint8 port, uint8 pin){
	DIO_SetupDirection(port,OUT,pin,NO_PR);
}

void buzzerON(uint8 port, uint8 pin){
	DIO_WritePin(port,LOGIC_HIGH,pin);
}

void buzzerOFF(uint8 port, uint8 pin){
		DIO_WritePin(port,LOGIC_LOW,pin);
}