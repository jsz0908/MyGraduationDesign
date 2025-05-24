#include "eeprom.h"
#include "i2c.h"
#include "delay.h"

#define EEPROM_ADDR_WRITE 0xA0
#define EEPROM_ADDR_READ  0xA1

void EEPROM_WriteData(unsigned char address, unsigned char database) {
    I2C_Start();
    I2C_SendByte(EEPROM_ADDR_WRITE);
    I2C_WaitAck();

    I2C_SendByte(address);
    I2C_WaitAck();

    I2C_SendByte(database);
    I2C_WaitAck();

    I2C_Stop();
    Delay_ms(5);  // EEPROM д����ʱ
}




unsigned char EEPROM_ReadData(unsigned char address) {
    unsigned char database;

    I2C_Start();
    I2C_SendByte(EEPROM_ADDR_WRITE);
    I2C_WaitAck();

    I2C_SendByte(address);
    I2C_WaitAck();

    I2C_Start();  // ���� I2C
    I2C_SendByte(EEPROM_ADDR_READ);
    I2C_WaitAck();

    database = I2C_ReceiveByte();
    I2C_SendNack();  // ֻ��һ���ֽڣ����Է� NACK

    I2C_Stop();
    return database;
}

void EEPROM_WriteFloat(unsigned char addr, float val) {
		int i;
    unsigned char *p = (unsigned char *)&val;
    for (i = 0; i < 4; i++) {
        EEPROM_WriteData(addr + i, *(p + i));
    }
}

float EEPROM_ReadFloat(unsigned char addr) {
		int i;
    float val;
    unsigned char *p = (unsigned char *)&val;
    for (i = 0; i < 4; i++) {
        *(p + i) = EEPROM_ReadData(addr + i);
    }
    return val;
}

void EEPROM_WriteInt(unsigned char addr, unsigned int val) {
    EEPROM_WriteData(addr, (val >> 8) & 0xFF);  // ���ֽ�
    EEPROM_WriteData(addr + 1, val & 0xFF);     // ���ֽ�
}

unsigned int EEPROM_ReadInt(unsigned char addr) {
    return (EEPROM_ReadData(addr) << 8) | EEPROM_ReadData(addr + 1);
}

