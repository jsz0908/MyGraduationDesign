#include <reg52.h>
#include "hx711.h"
#include <intrins.h>

sbit HX711_DT = P1^3;
sbit HX711_SCK = P1^2;

long OFFSET = 0;
float SCALE = 9000.0;  //故意定这么大，方便模拟现实生活人类的体重

void HX711_Delay() {
    _nop_(); _nop_();
}

unsigned long HX711_ReadRaw() {
    unsigned long count = 0;
    unsigned char i;

    HX711_DT = 1;
    HX711_SCK = 0;

    while (HX711_DT);

    for (i = 0; i < 24; i++) {
        HX711_SCK = 1;
        count <<= 1;
        HX711_SCK = 0;
        if (HX711_DT)
            count++;
        HX711_Delay();
    }

    HX711_SCK = 1;
    count ^= 0x800000;
    HX711_SCK = 0;

    return count;
}

void HX711_Tare() {
    OFFSET = HX711_ReadRaw();
}

float HX711_GetWeight() {
    long val = HX711_ReadRaw();
    return (float)(val - OFFSET) / SCALE;
}
