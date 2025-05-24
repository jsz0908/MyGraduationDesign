#include "led.h"
#include <reg52.h>

sbit RED_LED    = P3^7;
sbit YELLOW_LED = P3^6;
sbit GREEN_LED  = P3^5;


void UpdateBMILeds(float bmi) {
    if (bmi < 18.5) {
        RED_LED = 1;  
				YELLOW_LED = 0;
				GREEN_LED = 0;
				
    } else if (bmi >= 18.5 && bmi <= 23.9) {
        YELLOW_LED = 1;   // е§ГЃ
				RED_LED = 0;  
				GREEN_LED = 0;
    } else {
        RED_LED = 1;     // ГЌжи
				YELLOW_LED = 0;
				GREEN_LED = 0;
    }
}

void TurnOffLeds(void) {
    RED_LED = 0;
    YELLOW_LED = 0;
    GREEN_LED = 0;
}


