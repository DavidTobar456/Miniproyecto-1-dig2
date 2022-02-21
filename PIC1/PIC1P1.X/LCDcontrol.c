/*
 * File:   LCDcontrol.c
 * Author: David
 *
 * Created on 3 de febrero de 2022, 03:59 AM
 */


#include <xc.h>
#include <stdint.h>
#include "LCDcontrol.h"
#define _XTAL_FREQ 8000000
#define dataPORT PORTB
#define pinE PORTDbits.RD6
#define pinRW PORTDbits.RD5
#define pinRS PORTDbits.RD7

//Se definen puertos

//----------------------------Enable--------------------------------------------
void LCD_executeInstruction(){
    pinE=1;
    __delay_us(500);
    pinE=0;
}
//------------------------------------------------------------------------------

//-----------------------------Clear Display------------------------------------
void LCD_clearDisplay(){
    PORTEbits.RE0=0;
    PORTEbits.RE1=0;
    dataPORT=0b000000001;
    LCD_executeInstruction();
    __delay_ms(2);
}
//------------------------------------------------------------------------------

//----------------------------Entry Mode Set------------------------------------
void LCD_entryMode_set(uint8_t inc_dec, uint8_t shift){
    pinRS=0;
    pinRW=0;
    dataPORT=0;
    dataPORT=0b100+(inc_dec<<1)+shift;
    LCD_executeInstruction();
    __delay_us(50);
}
//------------------------------------------------------------------------------

//----------------------------Display On/Off------------------------------------
void LCD_displayOnOff(uint8_t on_off, uint8_t cursor, uint8_t blink){
    pinRS=0;
    pinRW=0;
    dataPORT=0;
    dataPORT=0b1000+(on_off<<2)+(cursor<<1)+blink;
    LCD_executeInstruction();
    __delay_us(50);
}
//------------------------------------------------------------------------------

//-----------------------------Function Set-------------------------------------
void LCD_functionSet(uint8_t dataL, uint8_t lineNo, uint8_t dispFont){
    pinRS=0;
    pinRW=0;
    dataPORT=0;
    dataPORT=0b100000+(dataL<<4)+(lineNo<<3)+(dispFont<<2);
    LCD_executeInstruction();
    __delay_us(50);
}
//------------------------------------------------------------------------------

//--------------------------Set CGRAM Addres------------------------------------
void LCD_setCGRAM(uint8_t address){
    pinRS=0;
    pinRW=0;
    PORTA=address;
    LCD_executeInstruction();
    __delay_us(50);
}
//------------------------------------------------------------------------------

//-------------------------Set DDRAM Address------------------------------------
void LCD_setDDRAM(uint8_t address){
    pinRS=0;
    pinRW=0;
    dataPORT = 0b10000000 + address;
    LCD_executeInstruction();
    __delay_us(50);
}
//------------------------------------------------------------------------------

//-----------------------Write CGRAM or DDRAM-----------------------------------
void LCD_wRAM(uint8_t data){
    pinRS=1;//RS
    pinRW=0;//RW
    dataPORT = data;//data
    LCD_executeInstruction();
    __delay_us(50);
}
//------------------------------------------------------------------------------

//-----------------------Read CGRAM or DDRAM------------------------------------
void LCD_rRAM(uint8_t address){
    pinRS;
    pinRW;
    LCD_executeInstruction();
    __delay_us(50);
}
//------------------------------------------------------------------------------

//----------------------------Texto a LCD---------------------------------------
/*uint8_t texto_a_LCD(char letra){
    return 0;
}*/
//------------------------------------------------------------------------------

//-----------------------Inicialización de la LCD-------------------------------
void initLCD(void){
    //----------------------Seteo de Funcion------------------------------------
    //Delay de mas de 15 ms
    __delay_ms(17);
    LCD_functionSet(1,0,0);
    __delay_ms(5);
    LCD_functionSet(1,0,0);
    __delay_us(110);
    LCD_functionSet(1,0,0);
    LCD_functionSet(1,1,0);
    LCD_displayOnOff(0,0,0);
    LCD_clearDisplay();
    LCD_entryMode_set(1,0);
    LCD_displayOnOff(1,1,1);
}
//------------------------------------------------------------------------------

//-----------------Funcion para escribir texto en el LCD------------------------
void LCDprintText(char texto[]){
    uint8_t i;
    for(i=0; i<strlen(texto); i++){
        LCD_wRAM(texto[i]);
    }
}
//------------------------------------------------------------------------------