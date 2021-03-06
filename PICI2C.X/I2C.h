/* File: I2C.h */
// obtenida de
//https://deepbluembedded.com/i2c-communication-protocol-tutorial-pic/
#ifndef I2C_H
#define I2C_H

#define _XTAL_FREQ             4000000
#define I2C_BaudRate           100000
//-----------[ Functions' Prototypes ]--------------

//---[ I2C Routines ]---
void I2C_Master_Init();
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Start(char add);
void I2C_Master_RepeatedStart();
void I2C_Master_Stop();
void I2C_ACK();
void I2C_NACK();
unsigned char I2C_Master_Write(unsigned char data);
unsigned char I2C_Read_Byte();
unsigned char I2C_Read(unsigned char);
#endif