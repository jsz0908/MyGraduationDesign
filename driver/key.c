#include "key.h"
#include <reg52.h>

sbit R1 = P2^3;
sbit R2 = P2^2;
sbit R3 = P2^1;
sbit R4 = P2^0;
sbit C1 = P2^4;
sbit C2 = P2^5;
sbit C3 = P2^6;
sbit C4 = P2^7;

const unsigned char KeyMap[4][4] = {
    {1,  2,  3,  4},
    {5,  6,  7,  8},
    {9, 10, 11, 12},
    {13,14,15,16}
};

void key_delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}

unsigned char KeyScan() {
    unsigned char row, col;

    for (row = 0; row < 4; row++) {
        // ���������ø�
        R1 = R2 = R3 = R4 = 1;

        // ��ǰ������
        switch (row) {
            case 0: R1 = 0; break;
            case 1: R2 = 0; break;
            case 2: R3 = 0; break;
            case 3: R4 = 0; break;
        }

        // ������
        if (C1 == 0) col = 0;
        else if (C2 == 0) col = 1;
        else if (C3 == 0) col = 2;
        else if (C4 == 0) col = 3;
        else continue;

        key_delay_ms(10); // ����

        // ȷ�ϰ����԰���
        if ((col == 0 && C1 == 0) || (col == 1 && C2 == 0) ||
            (col == 2 && C3 == 0) || (col == 3 && C4 == 0)) {

            // �ȴ��ɿ�
            while ((col == 0 && C1 == 0) || (col == 1 && C2 == 0) ||
                   (col == 2 && C3 == 0) || (col == 3 && C4 == 0));

            return KeyMap[row][col];
        }
    }

    return 0;
}
