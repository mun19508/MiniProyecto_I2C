/* File: I2C.h */
// obtenida de
//https://deepbluembedded.com/i2c-communication-protocol-tutorial-pic/
#ifndef I2C_H
#define I2C_H

#define _XTAL_FREQ             4000000
#define I2C_BaudRate           100000
//*****************************************************************************
// Funci�n para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init();
//*****************************************************************************
// Funci�n de espera: mientras se est� iniciada una comunicaci�n,
// est� habilitado una recepci�n, est� habilitado una parada
// est� habilitado un reinicio de la comunicaci�n, est� iniciada
// una comunicaci�n o se este transmitiendo, el IC2 PIC se esperar�
// antes de realizar alg�n trabajo
//*****************************************************************************
void I2C_Master_Wait();
//*****************************************************************************
// Funci�n de inicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_Start();
void I2C_Start(char add);
//*****************************************************************************
// Funci�n de reinicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart();
//*****************************************************************************
// Funci�n de parada de la comunicaci�n I2C PIC
//****************************************************************************
void I2C_Master_Stop();
void I2C_ACK();
void I2C_NACK();
//*****************************************************************************
//Funci�n de transmisi�n de datos del maestro al esclavo
//esta funci�n devolver� un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
unsigned char I2C_Master_Write(unsigned char data);
//*****************************************************************************
//Funci�n de recepci�n de datos enviados por el esclavo al maestro
//esta funci�n es para leer los datos que est�n en el esclavo
//*****************************************************************************
unsigned char I2C_Read_Byte();
unsigned char I2C_Read(unsigned char);
#endif