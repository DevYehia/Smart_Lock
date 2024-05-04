#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "../MCAL/DIO.h"
#include "../MCAL/TIMERS.h"
#include "../MCAL/SYSTICKTimer.h"


#define ULTRASONIC_TRIGGER_PORT PORT_A
#define ULTRASONIC_TRIGGER_PIN 	PIN4

#define ULTRASONIC_ECHO_PORT		PORT_B
#define ULTRASONIC_ECHO_PIN			PIN6

void ultrasonic_Init(void);
//uint32 Measure_distance(void);
void Delay(unsigned long counter);
//void ultrasonic_Enable_Interrupt(void);


#endif