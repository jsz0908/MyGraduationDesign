#include "lcd1602_i2c.h"
#include "i2c.h"
#include "delay.h"

#define LCD_ADDR 0x4E
#define LCD_BACKLIGHT 0x08
#define ENABLE  0x04
#define RS 0x01

void LCD_WriteByte(unsigned char database, unsigned char rs) {
    unsigned char high = database & 0xF0;
    unsigned char low = (database << 4) & 0xF0;
    unsigned char control = rs ? RS : 0x00;

    I2C_Start();
    I2C_SendByte(LCD_ADDR);
    I2C_WaitAck();
    I2C_SendByte(high | control | LCD_BACKLIGHT);
    I2C_WaitAck();
    I2C_SendByte(high | control | ENABLE | LCD_BACKLIGHT);
    I2C_WaitAck();
    I2C_SendByte(high | control | LCD_BACKLIGHT);
    I2C_WaitAck();
    I2C_Stop();

    I2C_Start();
    I2C_SendByte(LCD_ADDR);
    I2C_WaitAck();
    I2C_SendByte(low | control | LCD_BACKLIGHT);
    I2C_WaitAck();
    I2C_SendByte(low | control | ENABLE | LCD_BACKLIGHT);
    I2C_WaitAck();
    I2C_SendByte(low | control | LCD_BACKLIGHT);
    I2C_WaitAck();
    I2C_Stop();

    Delay_ms(2);
}

void LCD_WriteCmd(unsigned char cmd) {
    LCD_WriteByte(cmd, 0);
}

void LCD_WriteData(unsigned char database) {
    LCD_WriteByte(database, 1);
}

void LCD_SetCursor(unsigned char row, unsigned char col) {
    LCD_WriteCmd((row == 0 ? 0x80 : 0xC0) + col);
}

void LCD_ShowChar(unsigned char row, unsigned char col, char chr) {
    LCD_SetCursor(row, col);
    LCD_WriteData(chr);
}

void LCD_ShowString(unsigned char row, unsigned char col, char *str) {
    LCD_SetCursor(row, col);
    while (*str) {
        LCD_WriteData(*str++);
    }
}

void LCD_Init(void) {
		I2C_Init();
		Delay_ms(50);

		LCD_WriteCmd(0x33);  // 初始化流程
		LCD_WriteCmd(0x32);
		LCD_WriteCmd(0x28);  // 4位模式 2行 5x7 点阵
		LCD_WriteCmd(0x0C);  // 显示开，光标关
		LCD_WriteCmd(0x06);  // 写入后地址自动+1
		LCD_WriteCmd(0x01);  // 清屏
		Delay_ms(5);
}

void LCD_Clear(void) {
    LCD_WriteCmd(0x01);  // 清屏命令
    Delay_ms(5);         // 清屏后等待
}
