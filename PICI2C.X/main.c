/*
 * File:   main.c
 * Author: Daniel Mundo
 *
 * Created on 25 de febrero de 2021, 05:10 PM
 */
//**********************************Librerias***********************************
#include <xc.h>
#include <stdint.h>
#include "I2C.h"
#include "UART.h"
//******************************************************************************
//******************************Definiciones************************************
#define _XTAL_FREQ 4000000
#define PilotoLEDs PORTA
//talves defina el inicio de las direcciones.
//y el inicio de la comunicacion UART.
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
//********************************Variable**************************************
uint16_t Gx;
uint16_t Gy;
uint16_t Gz; //recupera los valores del giroscopio.
uint16_t Px;
uint16_t Py;
uint16_t Pz; //recupera los valores de la posicion.
uint16_t temperature; //recupera el valor de la temperatura
char* charEjex;
//******************************************************************************
//***************************Código de Interrupción***************************** 
void __interrupt() isr(void) {
    if (PIR1bits.RCIF) {
        char opcionUART = RCREG;
        switch (opcionUART) {
//---------------------------Cambios en el PIC----------------------------------            
            case 'A': //Coloca ambos LEDs en 0
                PilotoLEDs = 0;
                break;
            case 'B': //Coloca el LED en el pin RA0 en 1 y el pin RA1 en 0  
                PilotoLEDs = 1;
                break;
            case 'C': //Coloca ambos LED pin RA1 en 1 y el pin RA0 en 0
                PilotoLEDs = 2;
                break;
            case 'D': //Coloca ambos LEDs en 1
                PilotoLEDs = 3;
                break;
//---------------------------Cambios en Adafruit IO-----------------------------                
            case 'X': //envia el valor actual de la pos. en el eje x
                UARTSendString(charEjex,5);
                break;
        }
    }
}
//******************************************************************************
void main(void) {
    //--------------------------Canal Analogico---------------------------------
    ANSEL = 0;
    ANSELH = 0; //Puerto A y B como digitales
    //--------------------------Comunicacion UART-------------------------------
    UARTInit(9600, 1);
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    TRISCbits.TRISC6 = 0; //se habilita como salida el TX
    TRISCbits.TRISC7 = 1; //se habilita como entrada el RX   
    //--------------------------Puerto Entrada/salida---------------------------
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0; //ambos se habilitan para las luces piloto
    //-------------------------------Limpieza de puertos------------------------   
    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 0; //Se limpian los puertos
    //--------------------------Comunicacion SPI--------------------------------
    I2C_Master_Init();
    //--------------------------Loop principal----------------------------------
    while (1) {

    }
}