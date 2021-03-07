//Comentarios obtenidos de la libreria modificada de Electrosome
#include <xc.h>
#include "I2C.h"
//*****************************************************************************
// Función para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init() {
    SSPCON = 0x28;
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ / 4) / I2C_BaudRate) - 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait() {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start() {
    I2C_Master_Wait();
    SEN = 1;
}
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Start(char add) {
    I2C_Master_Wait();
    SEN = 1;
    I2C_Master_Write(add);
}
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//****************************************************************************
void I2C_Master_RepeatedStart() {
    I2C_Master_Wait();
    RSEN = 1;
}
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Stop() {
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_ACK(void) {
    ACKDT = 0; // 0 -> ACK
    ACKEN = 1; // Send ACK
    while (ACKEN);
}

void I2C_NACK(void) {
    ACKDT = 1; // 1 -> NACK
    ACKEN = 1; // Send NACK
    while (ACKEN);
}
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
unsigned char I2C_Master_Write(unsigned char data) {
    I2C_Master_Wait();
    SSPBUF = data;
    while (!SSPIF); // Wait Until Completion
    SSPIF = 0;
    return ACKSTAT;
}
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
unsigned char I2C_Read_Byte(void) {
    //---[ Receive & Return A Byte ]---
    I2C_Master_Wait();
    RCEN = 1; // Enable & Start Reception
    while (!SSPIF); // Wait Until Completion
    SSPIF = 0; // Clear The Interrupt Flag Bit
    I2C_Master_Wait();
    return SSPBUF; // Return The Received Byte
}

unsigned char I2C_Read(unsigned char ACK_NACK) {
    I2C_Master_Wait();
    //---[ Receive & Return A Byte & Send ACK or NACK ]---
    unsigned char Data;
    RCEN = 1;
    while (!BF);
    Data = SSPBUF;
    if (ACK_NACK == 0)
        I2C_ACK();
    else
        I2C_NACK();
    while (!SSPIF);
    SSPIF = 0;
    return Data;
}