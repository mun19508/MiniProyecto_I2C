#include <xc.h>
#include "I2C.h"
//---------------[ I2C Routines ]-------------------
//--------------------------------------------------

void I2C_Master_Init() {
    SSPCON = 0x28;
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ / 4) / I2C_BaudRate) - 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void I2C_Master_Wait() {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start() {
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Start(char add) {
    I2C_Master_Wait();
    SEN = 1;
    I2C_Master_Write(add);
}

void I2C_Master_RepeatedStart() {
    I2C_Master_Wait();
    RSEN = 1;
}

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

unsigned char I2C_Master_Write(unsigned char data) {
    I2C_Master_Wait();
    SSPBUF = data;
    while (!SSPIF); // Wait Until Completion
    SSPIF = 0;
    return ACKSTAT;
}

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