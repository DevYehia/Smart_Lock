#ifndef BUZZER_H_
#define BUZZER_H_

#include "../MCAL/DIO.h"

#define BUZZER_PORT PORT_B
#define BUZZER_PIN PIN2

void buzzerInit(uint8 port, uint8 pin);
void buzzerON(uint8 port,uint8 pin);
void buzzerOFF(uint8 port,uint8 pin);



#endif /* BUZZER_H_ */