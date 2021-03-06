/*
 * File:   main.c
 * Author: Daniel Mundo
 *
 * Created on 25 de febrero de 2021, 05:10 PM
 */
//**********************************Librerias***********************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "UART.h"
//******************************************************************************
//******************************Definiciones************************************
#define _XTAL_FREQ 4000000
#define PilotoLEDs PORTA
#define Sample_Rate_Divider 0x19
#define CONFIG_MPU6050 0x1A
#define Gyro_CONFIG 0x1B
#define Accel_CONFIG 0x1C
#define Power_Managment_1 0x6B
#define Accel_Xout_H 0x3B
#define MPU_Write 0xD0
#define MPU_Read 0xD1
//******************************************************************************
//***************************Bits de ConFig*************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//******************************************************************************
//**************************Prototipos de Funciones*****************************
void I2C_MPU_Init(void);
void I2C_Read_MPU(float* data_send);
//********************************Variable**************************************
int valor_original;
float valor_arreglado;
int status;
char* buffer;
//******************************************************************************
//***************************Código de Interrupción***************************** 

void __interrupt() isr(void) {
    if (PIR1bits.RCIF) {
        char opcionUART = RCREG;
        switch (opcionUART) {
                //---------------------------Cambios en el PIC----------------------------------            
            case 'a': //Coloca ambos LEDs en 0
                PilotoLEDs = 0;
                break;
            case 'b': //Coloca el LED en el pin RA0 en 1 y el pin RA1 en 0  
                PilotoLEDs = 1;
                break;
            case 'c': //Coloca ambos LED pin RA1 en 1 y el pin RA0 en 0
                PilotoLEDs = 2;
                break;
            case 'd': //Coloca ambos LEDs en 1
                PilotoLEDs = 3;
                break;
        }
    }
}
//******************************************************************************

void main(void) {
    __delay_ms(1000);
    //--------------------------Canal Analogico---------------------------------
    ANSEL = 0;
    ANSELH = 0; //Puerto A y B como digitales
    //--------------------------Comunicacion UART-------------------------------
    UARTInit(9600, 1);
    //--------------------------Puerto Entrada/salida---------------------------
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0; //Ambos pines de las luces piloto se habilitan  
    //-------------------------------Limpieza de puertos------------------------   
    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 0; //Se limpian los puertos
    //--------------------------Comunicacion I2C--------------------------------
    I2C_Master_Init();
    I2C_MPU_Init();
    //--------------------------Loop principal----------------------------------
    while (1) {
        I2C_Start(MPU_Write);
        while (SSPCON2bits.ACKSTAT);
        I2C_Master_Write(Accel_Xout_H); //Registro donde se inicia la lectura de valores
        while (SSPCON2bits.ACKSTAT);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(MPU_Read);
        valor_original = (((int) I2C_Read(0) << 8) | ((I2C_Read(1))));
        valor_arreglado = ((float) valor_original) * 0.0005982; 
        buffer = ftoa(valor_arreglado, status);
        UARTSendString(buffer, 6); //solo 5 cifras se envian

        UARTSendChar('\n');
    }
}

void I2C_MPU_Init(void) {
    //Config del modo de energia y reloj 
    I2C_Master_Start();
    I2C_Master_Write(MPU_Write); //Direccion de escritura 0xD0
    I2C_Master_Write(Power_Managment_1); //Es el para el registro con direccion 0x6B
    I2C_Master_Write(0x01); //PLL ref en el eje x
    I2C_Master_Stop();
    //Config de la frecuencia de los datos
    I2C_Master_Start();
    I2C_Master_Write(MPU_Write); //Direccion de escritura
    I2C_Master_Write(Sample_Rate_Divider); //Es el para el registro con direccion 0x19
    I2C_Master_Write(0x07); //Es el para que los datos tengan una frecuencia de 1KHz
    I2C_Master_Stop();
    //Config general 
    I2C_Master_Start();
    I2C_Master_Write(MPU_Write); //Direccion de escritura
    I2C_Master_Write(CONFIG_MPU6050); //Es el para el registro con direccion 0x1A
    I2C_Master_Write(0x00); //Input desactivada, maxima ancho de banda para el accel
    I2C_Master_Stop();
    //Config del giroscopio
    I2C_Master_Start();
    I2C_Master_Write(MPU_Write); //Direccion de escritura
    I2C_Master_Write(Gyro_CONFIG); //Es el para el registro con direccion 0x1B
    I2C_Master_Write(0x00); //Sin pruebas, +-250°/s  
    I2C_Master_Stop();
    //Config del acelerometro
    I2C_Master_Start();
    I2C_Master_Write(MPU_Write); //Direccion de escritura
    I2C_Master_Write(Accel_CONFIG); //Es el para el registro con direccion 0x1C
    I2C_Master_Write(0x00); //Sin pruebas y escala de +-2g
    I2C_Master_Stop();
    return;
}

