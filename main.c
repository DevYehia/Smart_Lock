#include "HAL/BLUETOOTH.h"
#include "HAL/ULTRASONIC.h"
#include "HAL/BUZZER.h"
#include "tm4c123gh6pm.h" 
#define PASSWORD_LENGTH 4

#define BUZZER_PORT PORT_B
#define BUZZER_PIN PIN2

#define ULTRASONIC_TRIGGER_PORT PORT_A
#define ULTRASONIC_TRIGGER_PIN 	PIN4

#define ULTRASONIC_ECHO_PORT		PORT_B
#define ULTRASONIC_ECHO_PIN			PIN6

uint8 password[PASSWORD_LENGTH] = "1234";
uint8 input[PASSWORD_LENGTH];


/* FOR ULTRASONIC CONFIG */
/* global variables to store and display distance in cm */
uint32 time; /*stores pulse on time */
uint32 distance; /* stores measured distance value */
uint8 lastedgeIn_flag =0;
int lastEdge, thisEdge;
unsigned long i = 0;

SysTickDisable();
SysTickPeriodSet(PERIOD_DELAY_10us,uS_UNIT); //10 micro second
Timer1_init();
Timer1_setDelay(); //60 ms 


enableInterruptToProcessor();
TIMER0_IMR_R |= 0x04;//Enable the timer0 interrupt 
Set_Bit(NVIC_EN0_R,19); //Enable the timer0 interrupt 
Timer1_enableInterrupt(); //Enable the timer1 interrupt  
SysTickEnable(WITH_INTERRUPT);

DIO_WritePin(ULTRASONIC_TRIGGER_PORT,LOGIC_HIGH,ULTRASONIC_TRIGGER_PIN); //Trigger ultrasoonic after pulse wave ends

void init(void){
	bluetooth_Init();
	ultrasonic_Init();
	buzzerInit(BUZZER_PORT,BUZZER_PIN);
}

uint8 getPassword(void){
	char c;
    for(uint8 cntr = 0 ; cntr < PASSWORD_LENGTH ; cntr++){
		c = bluetooth_Read();
		input[cntr] = c;
	}
	for(uint8 i = 0 ; i < PASSWORD_LENGTH ; i++){
		if(input[i] != password[i]){
			return 0;
		}
	}
	return 1;
}



int main(){
	init();
	bluetooth_Write_String("Please Enter the Password: ");
	int retries = 0;
	while(1){
		while(retries < 3){
			if(getPassword() == 1){
				bluetooth_Write_String("Welcome Home");
				break;
			}
			bluetooth_Write_String("Wrong Password Try Again!");
			retries++;
		}
		if(retries == 3){
			// Start Alarm
		}
	}
	
	
	
	
}

void SystickHandler(void){

  
  uint32 i=0;
  i=NVIC_ST_CTRL_R; //No need to read for clearing the interrupt in systick!!
  DIO_WritePin(ULTRASONIC_TRIGGER_PORT,LOGIC_LOW,ULTRASONIC_TRIGGER_PIN);
  SysTickDisable();
  Timer1_start();
  systick_done_flag=1;
 

}


void TIMER0_HANDLER(void){
  
  if(Get_Bit(GPIO_PORTB_DATA_R,ULTRASONIC_ECHO_PIN))
  {
    lastEdge = TIMER0_TAR_R;     /* save the timestamp */
		lastedgeIn_flag =1;
  }     
      /* detect falling edge */
  else if((!(Get_Bit(GPIO_PORTB_DATA_R,ULTRASONIC_ECHO_PIN))) && lastedgeIn_flag)     
  {
    thisEdge = TIMER0_TAR_R;     /* save the timestamp */
    time=(thisEdge - lastEdge); /* return the time difference */
    distance = (time * 10625)/10000000;
    if (distance >5) //an object is far
      {
        DIO_WritePin(PORT_F,LOGIC_HIGH,PIN1);
        DIO_WritePin(PORT_F,LOGIC_LOW,PIN3);
      }
      else
      {
        DIO_WritePin(PORT_F,LOGIC_HIGH,PIN3);
        DIO_WritePin(PORT_F,LOGIC_LOW,PIN1);
        
				buzzerON(BUZZER_PORT,BUZZER_PIN);
        bluetooth_Write_String("Someone is near, take care!!");
				
				
				
      }
   }
    TIMER0_ICR_R = 0x04;            /* clear timer0A capture flag */
}

void TIMER1_HANDLER(void){
  
  DIO_WritePin(ULTRASONIC_TRIGGER_PORT,LOGIC_HIGH,ULTRASONIC_TRIGGER_PIN);
  Timer1_stop();
  SysTickEnable(WITH_INTERRUPT);

  
  TIMER1_ICR_R = 0x01;
}