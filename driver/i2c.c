#include "i2c.h"
#include <reg52.h>
#include "delay.h"  // ������ʱͷ�ļ�

sbit SDA = P1^1;  // SDA
sbit SCL = P1^0;  // SCL

void I2C_Init(void) {
    // ���� SDA �� SCL ����Ϊ���
    P1 |= 0x03;  // ʹ P1.0 �� P1.1 ���
    Delay_ms(50);  // ��ʱ�ȴ��ȶ�
}

void I2C_Start(void) {
    SDA = 1;
    SCL = 1;
    SDA = 0;  // �����ź�
    SCL = 0;
}

void I2C_Stop(void) {
    SDA = 0;
    SCL = 1;
    SDA = 1;  // ֹͣ�ź�
}

void I2C_SendByte(unsigned char byte) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SDA = (byte & 0x80) ? 1 : 0;  // ���� MSB
        SCL = 1;
        SCL = 0;
        byte <<= 1;  // ����һλ��׼��������һ�� bit
    }
    // ���ԣ����� LED����ʾ������һ���ֽ�
    P1 = 0x01;  // ���� P1.0 �ǿ��� LED ������
}

void I2C_WaitAck(void) {
    SDA = 1;  // �ͷ� SDA
    SCL = 1;
    if (SDA == 1) {  // ���û��Ӧ��
        I2C_Stop();  // ֹͣͨ��
        // ���ԣ�δ�յ� ACK������ LED
        P1 = 0x02;  // ���� P1.1 ����һ������ LED ������
    }
    SCL = 0;
}


void I2C_SendAck(void) {
    SDA = 0;  // ����Ӧ��
    SCL = 1;
    SCL = 0;
}

void I2C_SendNack(void) {
    SDA = 1;  // ���ͷ�Ӧ��
    SCL = 1;
    SCL = 0;
}

unsigned char I2C_ReceiveByte(void) {
    unsigned char i, byte = 0;
    SDA = 1;  // �ͷ� SDA �Ա����

    for (i = 0; i < 8; i++) {
        SCL = 1;
        byte <<= 1;
        if (SDA)
            byte |= 0x01;
        SCL = 0;
    }
    return byte;
}

