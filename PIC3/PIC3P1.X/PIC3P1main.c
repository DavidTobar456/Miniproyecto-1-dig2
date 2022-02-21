/*
 * File:   PIC3P1main.c
 * Author: David Tobar
 *
 * Created on 16 de febrero, 02:12 AM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "../../../Laboratorio2/Laboratorio/Lab2dig2.X/confPIC16F887.h"

//-----------------------Prototipos de Funciones--------------------------------
void configuracion(void);
//------------------------------------------------------------------------------

//--------------------------Variables Globales----------------------------------
uint8_t cont;
//------------------------------------------------------------------------------

//-----------------------Codigo de Interrupcion---------------------------------
void __interrupt()isr(){
    if(INTCONbits.RBIF){
        if(!PORTBbits.RB0){
            cont--;
        }
        if(!PORTBbits.RB3){
            cont++;
        }
        INTCONbits.RBIF=0;
    }
    if(PIR1bits.SSPIF){
        SSPSTATbits.BF=0;
        SSPBUF=cont;
        PIR1bits.SSPIF=0;
    }
}
//------------------------------------------------------------------------------

//-------------------------------Main-------------------------------------------
void main(void) {
    configuracion();
    while(1){
        PORTD=cont;
    }
}
//------------------------------------------------------------------------------

//------------------------------Funciones---------------------------------------
void configuracion(void){
    //--------------------Configuracion de Puertos------------------------------
    //Las I/O se configuran como digitales
    ANSEL=0;
    ANSELH=0;
    //Se configuran PORTC y PORTA como salidas y PORTB como entrada
    TRISA=0;
    TRISB=0;
    TRISBbits.TRISB0=1;
    TRISBbits.TRISB3=1;
    TRISC=0;
    TRISD=0;
    //Se limpian los puertos habilitados
    PORTA=0;
    PORTC=0;
    PORTD=0;
    PORTB=0;
    //Se habilitan las resistencias pull-up en RB0 y RB1
    OPTION_REGbits.nRBPU=0;
    WPUBbits.WPUB0=1;
    WPUBbits.WPUB3=1;
    //--------------------------------------------------------------------------

    //------------------Configuracion del Oscilador-----------------------------
    setupOsc(_1MHz,_intOsc);
    //--------------------------------------------------------------------------

    //--------------------Configuracion del IOCB--------------------------------
    IOCBpinEnable(0);
    IOCBpinEnable(3);
    INTCONbits.RBIF=0;
    //--------------------------------------------------------------------------

    //--------------------Inicializacion del SPI--------------------------------
    initSPI_multiesclavo();
    //--------------------------------------------------------------------------

    //----------------Configuracion de Interrupciones---------------------------
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    //Se activan las interrupciones pertinentes
    IOCBenableInterrupt();
    SPIenableRInterrupt();
    //--------------------------------------------------------------------------

    //----------------Valores iniciales de Variables----------------------------
    cont=0;
    //--------------------------------------------------------------------------

    return;
}
//------------------------------------------------------------------------------
