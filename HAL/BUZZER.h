#ifndef BUZZER_H_
#define BUZZER_H_

#include "../MCAL/DIO.h"


void buzzerInit(uint8 port, uint8 pin);
void buzzerON(uint8 port,uint8 pin);
void buzzerOFF(uint8 port,uint8 pin);



#endif /* BUZZER_H_ */