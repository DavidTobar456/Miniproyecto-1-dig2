/*
 * File:   PIC1P1main.c
 * Author: David Tobar
 *
 * Created on 15 de febrero, 10:25 PM
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
#define _XTAL_FREQ 8000000

//-----------------------Prototipos de Funciones--------------------------------
void configuracion(void);
//------------------------------------------------------------------------------

//--------------------------Variables Globales----------------------------------
uint8_t medicion;
//------------------------------------------------------------------------------

//-----------------------Codigo de Interrupcion---------------------------------
void __interrupt()isr(){
    if(PIR1bits.ADIF){
        medicion=ADRESH;
        PIR1bits.ADIF=0;
    }
    if(PIR1bits.SSPIF){
        SSPSTATbits.BF=0;
        SSPBUF=medicion;
        PORTD=medicion;
        PIR1bits.SSPIF=0;
    }
}
//------------------------------------------------------------------------------

//-------------------------------Main-------------------------------------------
void main(void) {
    configuracion();
    while(1){
        PORTD=medicion;
        if(!ADCON0bits.GO){
            __delay_us(50);
            ADCstartMeasure();
        }
    }
}
//------------------------------------------------------------------------------

//------------------------------Funciones---------------------------------------
void configuracion(void){
    //--------------------Configuracion de Puertos------------------------------
    //Las I/O se configuran como digitales
    ANSEL=0;
    ANSELH=0;
    //Se configuran PORTC y PORTA como salidas
    TRISA=0;
    TRISC=0;
    TRISD=0;
    //Se limpian los puertos habilitados
    PORTA=0;
    PORTC=0;
    PORTD=0;
    //Se activan los puertos analogicos a utilizar
    ADCenableAnalogPort(_AN13);
    //--------------------------------------------------------------------------
    
    //------------------Configuracion del Oscilador-----------------------------
    setupOsc(_1MHz,_intOsc);
    //--------------------------------------------------------------------------
    
    //---------------------Configuracion del ADC--------------------------------
    setupADC(_Fosc2,_VddRef,_VssRef,_AN13,_leftAlign);
    //--------------------------------------------------------------------------
    
    //--------------------Inicializacion del SPI--------------------------------
    initSPI_multiesclavo();
    //--------------------------------------------------------------------------
    
    //----------------Configuracion de Interrupciones---------------------------
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    //Se activan las interrupciones pertinentes
    ADCenableInterrupt();
    SPIenableRInterrupt();
    //--------------------------------------------------------------------------
    
    //----------------Valores iniciales de Variables----------------------------
    medicion=0;
    //--------------------------------------------------------------------------
    
    return;
}
//------------------------------------------------------------------------------
