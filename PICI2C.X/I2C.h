/* File: I2C.h */
// obtenida de
//https://deepbluembedded.com/i2c-communication-protocol-tutorial-pic/
#ifndef I2C_H
#define I2C_H

#define _XTAL_FREQ             4000000
#define I2C_BaudRate           100000
//*****************************************************************************
// Función para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init();
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait();
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start();
void I2C_Start(char add);
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart();
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//****************************************************************************
void I2C_Master_Stop();
void I2C_ACK();
void I2C_NACK();
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
unsigned char I2C_Master_Write(unsigned char data);
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
unsigned char I2C_Read_Byte();
unsigned char I2C_Read(unsigned char);
#endif