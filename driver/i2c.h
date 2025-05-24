#ifndef __I2C_H__
#define __I2C_H__



// I2C ��ʼ��
void I2C_Init(void);

// I2C ������ֹͣ�ź�
void I2C_Start(void);
void I2C_Stop(void);

// I2C ���ݷ��ͺͽ���
void I2C_SendByte(unsigned char byte);
unsigned char I2C_ReadByte(unsigned char ack);

// I2C Ӧ����
void I2C_WaitAck(void);
void I2C_SendAck(void);
void I2C_SendNack(void);

unsigned char I2C_ReceiveByte(void);

#endif
