#ifndef __I2C_H__
#define __I2C_H__



// I2C 初始化
void I2C_Init(void);

// I2C 启动和停止信号
void I2C_Start(void);
void I2C_Stop(void);

// I2C 数据发送和接收
void I2C_SendByte(unsigned char byte);
unsigned char I2C_ReadByte(unsigned char ack);

// I2C 应答函数
void I2C_WaitAck(void);
void I2C_SendAck(void);
void I2C_SendNack(void);

unsigned char I2C_ReceiveByte(void);

#endif
