#include "lcd1602_i2c.h"
#include "key.h"
#include "hx711.h"
#include "delay.h"
#include "eeprom.h"
#include "led.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <REGX52.H>

sbit RED_LED    = P3^7;
sbit YELLOW_LED = P3^6;
sbit GREEN_LED  = P3^5;

unsigned char key = 0;
unsigned char current_page = 0;

bit state = 0;  // ��λ״̬��0Ϊkg��1Ϊg
xdata unsigned int height_cm = 0;

// ��������
void ShowMenu();
void ShowBMI();
void InputHeight();
void ShowWeight();
void TareScale();
void ShowHistory();
void ClearHistory();

void Start(){ 	
	unsigned int temp_COL = 0;  	
	LCD_ShowString(0, 0, "Starting...     "); 	
	while(temp_COL <= 12){ 		
		LCD_ShowString(1, temp_COL, ">>>>"); 		
		temp_COL += 4; 		
		Delay_ms(500); 	
	} 
} //ģ����һ������ui����


void main(void) {	
    TurnOffLeds();
    LCD_Init();
    HX711_Tare();     // Ĭ��ȥƤ
    height_cm = EEPROM_ReadInt(0x24);  // ����ʱ��ȡ��������
    Start();          //��������
    Delay_ms(500);
    ShowMenu();       // ��ʾ�˵�
    while (1) {
		TurnOffLeds();
        key = KeyScan();  // ɨ�谴��

        if (key == 1) {
            current_page = 1;
            ShowWeight();
        } else if (key == 2) {
            current_page = 2;
            TareScale();
            ShowMenu();
        } else if (key == 3) {
            current_page = 3;
            ShowHistory();
            Delay_ms(2000);
            ShowMenu();
        } else if (key == 4) {
            ClearHistory();
        } else if (key == 7) {
            current_page = 7;
            InputHeight();
        } else if (key == 8) {
            current_page = 8;
            ShowBMI();
        } else if (key == 0) {
            Delay_ms(50);  // ����ʱ��ʱ������CPU����
        }
    }
}

void ShowMenu() {
    LCD_Clear();
    LCD_ShowString(0, 0, "1.Wt 2.Tare 3.His");
    LCD_ShowString(1, 0, "4.Clr 7.Ht 8.BMI");
}

void ShowBMI() {
    char buf[16];
    float weight = EEPROM_ReadFloat(0x20);  // �ͱ���ʱ����һ��
    unsigned int height = EEPROM_ReadInt(0x24);
    float bmi = EEPROM_ReadFloat(0x26);

    LCD_Clear();
    LCD_ShowString(0, 0, "Weight:");
    sprintf(buf, "%.1fkg", weight);
    LCD_ShowString(0, 7, buf);

    LCD_ShowString(1, 0, "H:");
    sprintf(buf, "%dcm", height);
    LCD_ShowString(1, 2, buf);

    sprintf(buf, "BMI:%.1f", bmi);
    LCD_ShowString(1, 8, buf);
	
		TurnOffLeds();

    //BMI��������
//    if (bmi < 18.5) {
//        RED_LED = 1;  // ƫ��
//    } else if (bmi >= 18.5 && bmi <= 23.9) {
//        YELLOW_LED = 1;   // ����
//    } else {
//        RED_LED = 1;     // ����
//    }
		UpdateBMILeds(bmi);

    Delay_ms(2000);
    ShowMenu();
}


void InputHeight() {
    char buf[16];
    unsigned int temp = 0;
    height_cm = 0;
    LCD_Clear();
    LCD_ShowString(0, 0, "Input Height(cm):");

    while (1) {
        key = KeyScan();

        if (key >= 1 && key <= 10) {
            unsigned char num = (key == 10) ? 0 : key;
            if (height_cm < 1000) {
                height_cm = height_cm * 10 + num;
                sprintf(buf, "%d", height_cm);
                LCD_SetCursor(1, 0);
                LCD_ShowString(1, 0, "                ");
                LCD_SetCursor(1, 0);
                LCD_ShowString(1, 0, buf);
            }
        } else if (key == 11) {  // ȷ��
            break;
        } else if (key == 12) {  // ������������
            height_cm = height_cm / 10;
            sprintf(buf, "%d", height_cm);
            LCD_SetCursor(1, 0);
            LCD_ShowString(1, 0, "                ");
            LCD_SetCursor(1, 0);
            LCD_ShowString(1, 0, buf);
        }

        Delay_ms(150);
    }

    EEPROM_WriteInt(0x24, height_cm);  // �������
    LCD_Clear();
    LCD_ShowString(0, 0, "Height Saved!");
    Delay_ms(1000);
    ShowMenu();
}

void ShowWeight() {
    char buf[16];
    char last_buf[16] = "";
    LCD_Clear();
    LCD_ShowString(0, 0, "Weight:");

    while (1) {
        float weight = HX711_GetWeight();
        float bmi = 0;
				unsigned char i;
			  bmi = weight / ((height_cm / 100.0) * (height_cm / 100.0));
        

        if (state == 0) {
            sprintf(buf, "%.1f kg", weight);
        } else {
            sprintf(buf, "%.0f g", weight * 1000);
        }

        if (strcmp(buf, last_buf) != 0) {
            LCD_SetCursor(1, 0);
            for (i = 0; i < 16; i++) {
                LCD_WriteData(' ');
            }
            LCD_ShowString(1, 0, buf);
            strcpy(last_buf, buf);
        }

        UpdateBMILeds(bmi);  // ���� BMI ���� LED

        key = KeyScan();
        if (key != 0) {
            if (key == 5) {
                state = !state;
                last_buf[0] = '\0';
            } else if (key == 7) {
                float previous = EEPROM_ReadFloat(0x20);      // ��ǰ���أ���һ�ֲ�����
								EEPROM_WriteFloat(0x10, previous);            // �ȱ���ɡ���һ�����ء�
							
                EEPROM_WriteFloat(0x20, weight);  // ��ǰ����
                EEPROM_WriteInt(0x24, height_cm); // ���
                EEPROM_WriteFloat(0x26, bmi);     // BMI

                LCD_Clear();
                LCD_ShowString(0, 0, "Saved to EEPROM");
                Delay_ms(1000);
            } else {
                TurnOffLeds();  // �˳�ʱϨ���
                ShowMenu();
                break;
            }
        }

        Delay_ms(100);
    }
}





void TareScale() {
    LCD_Clear();
    LCD_ShowString(0, 0, "Taring...");
    HX711_Tare();
    Delay_ms(1000);
    LCD_Clear();
    LCD_ShowString(0, 0, "Done");
    Delay_ms(1000);
}

void ShowHistory() {
		char buf[16];
		float last_weight ;  
		last_weight	= EEPROM_ReadFloat(0x10);  // ��һ������
    LCD_Clear();
    LCD_ShowString(0, 0, "Last:");    
    sprintf(buf, "%.1fkg", last_weight);
    LCD_ShowString(0, 6, buf);

}

void ClearHistory() {
    EEPROM_WriteFloat(0x10, 0.0f);  // �����ʷ����
    LCD_Clear();
    LCD_ShowString(0, 0, "History Cleared");
    Delay_ms(1000);
    ShowMenu();
}


