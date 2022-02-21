/* 
 * File:   confPIC16F887.h
 * Author: David
 *
 * Created on 26 de enero de 2022, 02:44 AM
 */

#ifndef CONFPIC16F887_H
#define	CONFPIC16F887_H

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 8000000

//Canales analógicos
#define _AN0 0
#define _AN1 1
#define _AN2 2
#define _AN3 3
#define _AN4 4
#define _AN5 5
#define _AN6 6
#define _AN7 7
#define _AN8 8
#define _AN9 9
#define _AN10 10
#define _AN11 11
#define _AN12 12
#define _AN13 13

//Configuración de ADC
#define _rightAlign 1
#define _leftAlign 0
#define _VssRef 0
#define _VddRef 0
#define _VrefPin 1
#define _OnADC 1
#define _OffADC 0
#define _Fosc2 0b00
#define _Fosc8 0b01
#define _Fosc32 0b10
#define _Frc 0b11


//Configuración de Oscilador
#define _31kHz 0b000
#define _125kHz 0b001
#define _250kHz 0b010
#define _500kHz 0b011
#define _1MHz 0b100
#define _2MHz 0b101
#define _4MHz 0b110
#define _8MHz 0b111
#define _intOsc 1
#define _extOsc 0

//Configuracion de Timer0
#define _T0intOsc 0
#define _T0tPin 1
#define _1a2 0b000
#define _1a4 0b001
#define _1a8 0b010
#define _1a16 0b011
#define _1a32 0b100
#define _1a64 0b101
#define _1a128 0b110
#define _1a256 0b111

//Configuracion del SPI
#define _SPIfosc4 0
#define _SPIfosc16 1
#define _SPIfosc64 2
#define _SPItmr2 3

//----------------------Funciones asociadas al ADC------------------------------
void ADCenableInterrupt(void);
void ADCenableAnalogPort(uint8_t puerto);
void ADCchannelSelect(uint8_t canal);
void ADCstartMeasure(void);
void setupADC(uint8_t Fosc, uint8_t VrefP, uint8_t VrefN, uint8_t channel,
                uint8_t align);
//------------------------------------------------------------------------------

//---------------------Funciones asociadas al OSCCON----------------------------
void setupOsc(uint8_t freq, uint8_t clkSrc);
//------------------------------------------------------------------------------

//----------Funciones asociadas a la interrupción de puerto B-------------------
void IOCBenableInterrupt(void);
void IOCBpinEnable(uint8_t pin);
//------------------------------------------------------------------------------

//------------------Funciones asociadas a Timer0--------------------------------
void setupTimer0(uint8_t clkSrc, uint8_t prescaler, uint8_t carga);
void resTimer0(uint8_t carga);
void T0enableInterrupt(void);
//------------------------------------------------------------------------------

//-------------------Funciones asociadas a SPI----------------------------------
void initSPI_maestro(uint8_t SPIfosc);
void initSPI_esclavo(void);
void initSPI_multiesclavo(void);
//------------------------------------------------------------------------------

#endif	/* CONFPIC16F887_H */

