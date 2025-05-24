#ifndef __LCD1602_I2C_H__
#define __LCD1602_I2C_H__

void LCD_WriteByte(unsigned char database, unsigned char rs);
void LCD_WriteCmd(unsigned char cmd);
void LCD_WriteData(unsigned char database);
void LCD_SetCursor(unsigned char row, unsigned char col);

void LCD_Init(void);
void LCD_ShowChar(unsigned char row, unsigned char col, char chr);
void LCD_ShowString(unsigned char row, unsigned char col, char *str);
void LCD_Clear(void);

#endif
