#include "i2c.h"
#include <reg52.h>
#include "delay.h"  // 包含延时头文件

sbit SDA = P1^1;  // SDA
sbit SCL = P1^0;  // SCL

void I2C_Init(void) {
    // 设置 SDA 和 SCL 引脚为输出
    P1 |= 0x03;  // 使 P1.0 和 P1.1 输出
    Delay_ms(50);  // 延时等待稳定
}

void I2C_Start(void) {
    SDA = 1;
    SCL = 1;
    SDA = 0;  // 启动信号
    SCL = 0;
}

void I2C_Stop(void) {
    SDA = 0;
    SCL = 1;
    SDA = 1;  // 停止信号
}

void I2C_SendByte(unsigned char byte) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SDA = (byte & 0x80) ? 1 : 0;  // 发送 MSB
        SCL = 1;
        SCL = 0;
        byte <<= 1;  // 左移一位，准备发送下一个 bit
    }
    // 调试：点亮 LED，表示发送了一个字节
    P1 = 0x01;  // 假设 P1.0 是控制 LED 的引脚
}

void I2C_WaitAck(void) {
    SDA = 1;  // 释放 SDA
    SCL = 1;
    if (SDA == 1) {  // 如果没有应答
        I2C_Stop();  // 停止通信
        // 调试：未收到 ACK，点亮 LED
        P1 = 0x02;  // 假设 P1.1 是另一个控制 LED 的引脚
    }
    SCL = 0;
}


void I2C_SendAck(void) {
    SDA = 0;  // 发送应答
    SCL = 1;
    SCL = 0;
}

void I2C_SendNack(void) {
    SDA = 1;  // 发送非应答
    SCL = 1;
    SCL = 0;
}

unsigned char I2C_ReceiveByte(void) {
    unsigned char i, byte = 0;
    SDA = 1;  // 释放 SDA 以便接收

    for (i = 0; i < 8; i++) {
        SCL = 1;
        byte <<= 1;
        if (SDA)
            byte |= 0x01;
        SCL = 0;
    }
    return byte;
}

