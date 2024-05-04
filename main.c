#include "HAL/BLUETOOTH.h"
#include "HAL//ULTRASONIC.h"
#include "LIB/bitwise_operation.h"

#define PASSWORD_LENGTH 4
#define MAX_NUM_TRIES 3

#define BUZZER_PIN  5
#define BUZZER_PORT PORT_F

uint8 password[PASSWORD_LENGTH] = "1234";
uint8 input[PASSWORD_LENGTH];

void init(void){
	bluetooth_Init();
	ultrasonic_Init();
	Timer1_init();
	Timer1_setDelay();
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
			Timer1_start();
			while(Get_Bit(TIMER1_RIS_R,0) == 0);
			Set_Bit(TIMER1_ICR_R,0);
			Timer1_stop();
			retries = 0;
		}
		else{
			break;
		}
	}
	if (DIO_Init(PORT_F) == 0) {
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
                        case '1':
							if (DIO_SetupDirection(PORT_F, OUT, PIN1 , NO_PR) == 0) {
								if (DIO_WritePin(PORT_F, LOGIC_HIGH, PIN1) == 0)
									bluetooth_Write_String("Kitchen door opened successfully!");
							}
                            break;
                        case '2':
 							if (DIO_SetupDirection(PORT_F, OUT, PIN2 , NO_PR) == 0) {
								if (DIO_WritePin(PORT_F, LOGIC_HIGH, PIN2) == 0)
									bluetooth_Write_String("Garage door opened successfully!");
							}
                            break;
                        case '3':
 							if (DIO_SetupDirection(PORT_F, OUT, PIN3 , NO_PR) == 0) {
								if (DIO_WritePin(PORT_F, LOGIC_HIGH, PIN3) == 0)
									bluetooth_Write_String("Front door opened successfully!");
							}
                            break;
                        case '4':
 							if (DIO_SetupDirection(PORT_F, OUT, PIN1 , NO_PR) == 0 && DIO_SetupDirection(PORT_F, OUT, PIN2 , NO_PR) == 0) {
								if (DIO_WritePin(PORT_F, LOGIC_HIGH, PIN1) == 0 && DIO_WritePin(PORT_F, LOGIC_HIGH, PIN2) == 0)
									bluetooth_Write_String("Kitchen window opened successfully!");
							}
                            break;
                        case '5':
 							if (DIO_SetupDirection(PORT_F, OUT, PIN2 , NO_PR) == 0 && DIO_SetupDirection(PORT_F, OUT, PIN3 , NO_PR) == 0) {
								if (DIO_WritePin(PORT_F, LOGIC_HIGH, PIN2) == 0 && DIO_WritePin(PORT_F, LOGIC_HIGH, PIN3) == 0)
									bluetooth_Write_String("Outside window opened successfully!");
							}
                            break;
						case '6':
							if (DIO_WritePin(PORT_F, LOGIC_LOW, PIN1) == 0)
								bluetooth_Write_String("Kitchen door closed successfully!");
							break;
						case '7':
							if (DIO_WritePin(PORT_F, LOGIC_LOW, PIN2) == 0)
								bluetooth_Write_String("Garage door closed successfully!");
							break;
						case '8':
							if (DIO_WritePin(PORT_F, LOGIC_LOW, PIN3) == 0)
								bluetooth_Write_String("Front door closed successfully!");
							break;
						case '9':
							if (DIO_WritePin(PORT_F, LOGIC_LOW, PIN1) == 0 && DIO_WritePin(PORT_F, LOGIC_LOW, PIN2) == 0)
								bluetooth_Write_String("Kitchen window closed successfully!");
							break;
						case '10':
							if (DIO_WritePin(PORT_F, LOGIC_LOW, PIN2) == 0 && DIO_WritePin(PORT_F, LOGIC_LOW, PIN3) == 0)
								bluetooth_Write_String("Outside window closed successfully!");
							break;
					}
			}
	}
	
	
	
	
	
}