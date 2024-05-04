#include "HAL/BLUETOOTH.h"
#include "HAL//ULTRASONIC.h"

#define PASSWORD_LENGTH 4

uint8 password[PASSWORD_LENGTH] = "1234";
uint8 input[PASSWORD_LENGTH];

void init(void){
	bluetooth_Init();
	ultrasonic_Init();
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