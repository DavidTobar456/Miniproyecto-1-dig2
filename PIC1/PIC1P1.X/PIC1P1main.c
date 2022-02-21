/*
 * File:   PIC1P1main.c
 * Author: David Tobar
 *
 * Created on 14 de febrero, 04:10 PM
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
#include <string.h>
#include <pic16f887.h>
#include "../../../Laboratorio2/Laboratorio/../Laboratorio/Lab2dig2.X/confPIC16F887.h"
#include "LCDcontrol.h"
#define _XTAL_FREQ 8000000

//---------------------------Definiciones---------------------------------------
#define _SS1 1
#define _SS2 2
#define _SS3 3


//------------------------Prototipos de Funciones-------------------------------
void configuracion(void);
void separar_numero(int numero, uint8_t *centenas, uint8_t *decenas, uint8_t *unidades);
void selectSlave(uint8_t slave);
//------------------------------------------------------------------------------

//--------------------------Variables Globales----------------------------------
uint8_t pot;
uint8_t cont;
uint8_t temp;
uint8_t comunicacion;

struct variables{
    int mapeado;
    uint8_t centenas;
    uint8_t decenas;
    uint8_t unidades;
}potS, contS, tempS;
//------------------------------------------------------------------------------

//----------------------------Interrupciones------------------------------------
void __interrupt()isr(){
    if(PIR1bits.TMR1IF){
        comunicacion=1;
        resTimer1(3036);
    }
}
//------------------------------------------------------------------------------

//--------------------------------Main------------------------------------------
void main(void) {
    configuracion();
    initLCD();
    LCDprintText("S1:   S2:  S3:");
    while(1){
        //Slave Select, SLAVE1
        selectSlave(_SS1);
        SPIsendValue(0b11000011);
        __delay_us(200);
        SPIreadValue(&pot);
        //Slave Select, SLAVE2
        selectSlave(_SS2);
        SPIsendValue(0b11000011);
        __delay_us(200);
        SPIreadValue(&cont);
        //Slave Select, SLAVE3
        selectSlave(_SS3);
        SPIsendValue(0b11000011);
        __delay_us(200);
        SPIreadValue(&temp);
        
        potS.mapeado = (100*pot)/51;
        
        separar_numero(potS.mapeado, &potS.centenas, &potS.decenas, &potS.unidades);
        separar_numero(cont,&contS.centenas,&contS.decenas,&contS.unidades);
        separar_numero(temp,&tempS.centenas,&tempS.decenas,&tempS.unidades);
        
        char potTexto[]={potS.centenas+48,'.',potS.decenas+48,potS.unidades+48,'\0'};
        char contTexto[]={contS.centenas+48,contS.decenas+48,contS.unidades+48,'\0'};
        char tempTexto[]={tempS.centenas+48,tempS.decenas+48,tempS.unidades+48,'\0'};
        
        //Imprimir valorse en la LCD
        LCD_setDDRAM(0b1000000);
        LCDprintText(potTexto);
        LCD_wRAM('v');
        LCD_wRAM(' ');
        LCDprintText(contTexto);
        LCDprintText("  ");
        LCDprintText(tempTexto);
        LCD_wRAM('C');
        
        if(comunicacion){
             //Se imprime el siguiente texto en la terminal por el UART
            UARTprintText("S1:   S2:  S3:");
            UARTprintCharacter(13);
        
            UARTprintText(potTexto);
            UARTprintCharacter('v');

            UARTprintCharacter(' ');
            UARTprintText(contTexto);

            UARTprintText("  ");
            UARTprintText(tempTexto);
            UARTprintCharacter('C');

            UARTprintCharacter(13);
            UARTprintCharacter(13);
            
            UARTprintDigit(potS.centenas);
            UARTprintDigit(potS.decenas);
            UARTprintDigit(potS.unidades);
            
            UARTprintCharacter(13);
            UARTprintCharacter(13);
            
            comunicacion=0;
        }
        
        
    }
}
//------------------------------------------------------------------------------

//------------------------------Funciones---------------------------------------
void configuracion(void){
    //-------------------Configuracion de Puertos-------------------------------
    //Se configuran los puertos como digitales
    ANSEL=0;
    ANSELH=0;
    //Se configuran PORTA,PORTB,PORTC y PORTD como salidas
    TRISA=0;
    TRISB=0;
    TRISD=0;
    TRISC=0;
    TRISE=0;
    //Se limpian los puertos a utilizar
    PORTA=0;
    PORTB=0;
    PORTD=0;
    PORTC=0;
    PORTE=0;
    //--------------------------------------------------------------------------
    
    //-----------------Configuracion del Oscilador------------------------------
    setupOsc(_1MHz,_intOsc);
    //--------------------------------------------------------------------------
    
    //----------------------Configuracion SPI-----------------------------------
    initSPI_maestro(_SPIfosc4);
    //--------------------------------------------------------------------------
    
    //---------------------Configuracion USART----------------------------------
    initUSARTransmit(1,1,25);
    //--------------------------------------------------------------------------
    
    //------------------Configuracion del Timer0--------------------------------
    setupTimer1(0,1,3036);
    //--------------------------------------------------------------------------
    
    //---------------Configuracion de Interrupciones----------------------------
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    T1enableInterrupt();
    //--------------------------------------------------------------------------
    
    //---------------Valores iniciales de Variables-----------------------------
    pot=0;
    cont=0;
    temp=0;
    //--------------------------------------------------------------------------
    return;
}

void separar_numero(int numero, uint8_t *centenas, uint8_t *decenas, uint8_t *unidades){
    int numaux,numaux2;
    numaux = numero%100;
    if(numero == numaux){
        *centenas = 0;
    }else{
        *centenas = (numero - numaux)/100;
    }
    numaux2 = numaux%10;
    if(numaux == numaux2){
        *decenas = 0;
    }else{
        *decenas = (numaux - numaux2)/10;
    }
    *unidades = numaux2;
}

void selectSlave(uint8_t slave){
    switch(slave){
        case 1:
            PORTDbits.RD2=0;
            PORTAbits.RA0=1;
            PORTDbits.RD3=1;
            break;
        case 2:
            PORTDbits.RD2=1;
            PORTAbits.RA0=0;
            PORTDbits.RD3=1;
            break;
        case 3:
            PORTDbits.RD2=1;
            PORTAbits.RA0=1;
            PORTDbits.RD3=0;
            break;
        default:
            PORTDbits.RD2=1;
            PORTAbits.RA0=1;
            PORTDbits.RD3=1;
            break;
    }
}
//------------------------------------------------------------------------------