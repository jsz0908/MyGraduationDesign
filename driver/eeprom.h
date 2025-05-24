#ifndef __EEPROM_H__
#define __EEPROM_H__

void EEPROM_WriteData(unsigned char address, unsigned char database);
unsigned char EEPROM_ReadData(unsigned char address);

void EEPROM_WriteFloat(unsigned char addr, float val);
float EEPROM_ReadFloat(unsigned char addr);
void EEPROM_WriteInt(unsigned char addr, unsigned int val);
unsigned int EEPROM_ReadInt(unsigned char addr);

#endif
