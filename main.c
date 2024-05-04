#include "HAL/BLUETOOTH.h"
#include "HAL/ULTRASONIC.h"
#include "HAL/BUZZER.h"
#include "tm4c123gh6pm.h" 
#define PASSWORD_LENGTH 4
#define MAX_NUM_TRIES 3


#define DOORS_PORT         PORT_D
#define WINDOWS_PORT       PORT_E


#define KITCHEN_DOOR_LED   PIN0
#define FRONT_DOOR_LED     PIN1
#define GARAGE_DOOR_LED    PIN2

#define KITCHEN_WINDOW_LED PIN1
#define OUTSIDE_WINDOW_LED PIN2

uint8 password[PASSWORD_LENGTH] = "1234";
uint8 input[PASSWORD_LENGTH];


void initLEDs(void){
	DIO_Init(DOORS_PORT);
	DIO_Init(WINDOWS_PORT);
	DIO_SetupDirection(DOORS_PORT,OUT,KITCHEN_DOOR_LED,NO_PR);
	DIO_SetupDirection(DOORS_PORT,OUT,GARAGE_DOOR_LED,NO_PR);
	DIO_SetupDirection(DOORS_PORT,OUT,FRONT_DOOR_LED,NO_PR);
	DIO_SetupDirection(WINDOWS_PORT,OUT,KITCHEN_WINDOW_LED,NO_PR);
	DIO_SetupDirection(WINDOWS_PORT,OUT,OUTSIDE_WINDOW_LED,NO_PR);
}


void init(void){
	
	initLEDs();
	SysTickDisable();
	SysTickPeriodSet(PERIOD_DELAY_10us,uS_UNIT); //10 micro second
	Timer1_init();
	Timer1_setDelay(); //60 ms 
	bluetooth_Init();
	ultrasonic_Init();
	Timer2_init();
	Timer2_setDelay(); //1 minute
	buzzerInit(BUZZER_PORT,BUZZER_PIN);
	TIMER0_IMR_R |= 0x04;//Enable the timer0 interrupt 
	Set_Bit(NVIC_EN0_R,19); //Enable the timer0 interrupt 
	Timer1_enableInterrupt(); //Enable the timer1 interrupt  
	SysTickEnable(WITH_INTERRUPT);
	DIO_WritePin(ULTRASONIC_TRIGGER_PORT,LOGIC_HIGH,ULTRASONIC_TRIGGER_PIN); //Trigger ultrasoonic after pulse wave ends
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
		while(retries < MAX_NUM_TRIES){
			if(getPassword() == 1){
				bluetooth_Write_String("Welcome Home\n");
				break;
			}
			bluetooth_Write_String("Wrong Password Try Again!\n");
			retries++;
		}
		if(retries == 3){
			Timer2_start();
			while(Get_Bit(TIMER2_RIS_R,0) == 0);
			Set_Bit(TIMER2_ICR_R,0);
			Timer2_stop();
			retries = 0;
		}
		else{
			break;
		}
	}
	while(1) {
		bluetooth_Write_String("  0: open kitchen door     1: open garage door     2: open front door\n");
		bluetooth_Write_String("  3: open kitchen window   4: open outside window  5: close kitchen door\n");
		bluetooth_Write_String("  6: close garage door     7: close front door     8: close kitchen window\n");
		bluetooth_Write_String("  9: close outside window\n");
	    char choice;
		while (1) {
			char choice = bluetooth_Read(); 
			if (choice >= '0' && choice <= '9') { 
							break; 
			}
			else{
				bluetooth_Write_String("Wrong Option, Try Again!!\n");
			}
		}

        switch (choice) {
                        case '0':
							if (DIO_WritePin(DOORS_PORT, LOGIC_HIGH, KITCHEN_DOOR_LED) == 0)
								bluetooth_Write_String("Kitchen door opened successfully!");
                            break;
                        case '1':
							if (DIO_WritePin(DOORS_PORT, LOGIC_HIGH, GARAGE_DOOR_LED) == 0)
								bluetooth_Write_String("Garage door opened successfully!");
                            break;
                        case '2':
							if (DIO_WritePin(DOORS_PORT, LOGIC_HIGH, FRONT_DOOR_LED) == 0)
								bluetooth_Write_String("Front door opened successfully!");
                            break;
                        case '3':
							if (DIO_WritePin(WINDOWS_PORT, LOGIC_HIGH, KITCHEN_WINDOW_LED) == 0)
								bluetooth_Write_String("Kitchen window opened successfully!");
                            break;
                        case '4':
							if (DIO_WritePin(WINDOWS_PORT, LOGIC_HIGH, OUTSIDE_WINDOW_LED) == 0)
								bluetooth_Write_String("Outside window opened successfully!");
                            break;
						case '5':
							if (DIO_WritePin(DOORS_PORT, LOGIC_LOW, KITCHEN_DOOR_LED) == 0)
								bluetooth_Write_String("Kitchen door closed successfully!");
							break;
						case '6':
							if (DIO_WritePin(DOORS_PORT, LOGIC_LOW, GARAGE_DOOR_LED) == 0)
								bluetooth_Write_String("Garage door closed successfully!");
							break;
						case '7':
							if (DIO_WritePin(DOORS_PORT, LOGIC_LOW, FRONT_DOOR_LED) == 0)
								bluetooth_Write_String("Front door closed successfully!");
							break;
						case '8':
							if (DIO_WritePin(WINDOWS_PORT, LOGIC_LOW, KITCHEN_WINDOW_LED) == 0)
								bluetooth_Write_String("Kitchen window closed successfully!");
							break;
						case '9':
							if (DIO_WritePin(WINDOWS_PORT, LOGIC_LOW, OUTSIDE_WINDOW_LED) == 0 )
								bluetooth_Write_String("Outside window closed successfully!");
							break;
					}
			}
}



/* FOR ULTRASONIC CONFIG */
/* global variables to store and display distance in cm */
uint32 time; /*stores pulse on time */
uint32 distance; /* stores measured distance value */
uint8 lastedgeIn_flag =0;
int lastEdge, thisEdge;
unsigned long i = 0;



void SystickHandler(void){

  
  uint32 i=0;
  i=NVIC_ST_CTRL_R; //No need to read for clearing the interrupt in systick!!
  DIO_WritePin(ULTRASONIC_TRIGGER_PORT,LOGIC_LOW,ULTRASONIC_TRIGGER_PIN);
  SysTickDisable();
  Timer1_start();
 

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
		buzzerOFF(BUZZER_PORT,BUZZER_PIN);
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