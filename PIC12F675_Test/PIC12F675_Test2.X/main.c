/*
 * File:   main.c
 * Author: Apoca
 *
 * Created on 2021. április 20., 21:31
 */

 
#include <xc.h>
#include <stdio.h>
#include "PIC12F675.h"
 
#pragma config FOSC=INTRCIO,WDTE=OFF,MCLRE=OFF,BOREN=OFF


//44 us -10 = 245 / 000
//42 us -9 = 246 / 000
//100 us -50 = 205 / 000
//1000 us -125 = 130 / 010
#define TIMER0_CNT_VAL 205

#define _XTAL_FREQ 4000000

unsigned char iADval = 0;
unsigned char iLSpeed = 0;
unsigned char iRSpeed = 0;
unsigned char iCnt = 0;
//__eeprom unsigned char eeADH;
//__eeprom unsigned char eeADL;

void __interrupt() ISR(void)
{
// Timer0 Interrupt - Freq = 38461.54 Hz - Period = 0.000026 seconds
    if (INTCONbits.TMR0IF ==1) // timer 0 interrupt flag
    {
        //GPIObits.GPIO5 = ~GPIObits.GPIO5;      // Toggle PORTB bit0 LED
        INTCONbits.TMR0IF = 0;         // clear the flag
        INTCONbits.TMR0IE = 1;         // reenable the interrupt
        TMR0 = TIMER0_CNT_VAL;           // reset the timer preset count
        ++iCnt;
        
        if (iCnt >= iLSpeed)
            GPIObits.GPIO5 = 0;
        else
            GPIObits.GPIO5 = 1;
        
        if (iCnt >= iRSpeed)
            GPIObits.GPIO4 = 0;
        else
            GPIObits.GPIO4 = 1;
    }

}
 
void main(void)
{
    ANSEL = 0;
    // setup portb to show the interrupts by blibking LEDs
    
    TRISIObits.TRISIO5 = 0;    // PORT is GPIO5 output...to show the interrupts
    GPIObits.GPIO5 = 0;       // start with GPIO5 outputs low
    TRISIObits.TRISIO4 = 0;    // PORT is GPIO4 output...to show the interrupts
    GPIObits.GPIO4 = 0;       // start with GPIO4 outputs low

    //Timer0 Registers Prescaler= 1 - TMR0 Preset = 204 - Freq = 38461.54 Hz - Period = 0.000026 seconds
    OPTION_REGbits.T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
    OPTION_REGbits.PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the WDT
    OPTION_REGbits.PS2 = 0;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = TIMER0_CNT_VAL;             // preset for timer register

    // Interrupt Registers
    INTCON = 0;           // clear the interrpt control register
    INTCONbits.TMR0IE = 1;        // bit5 TMR0 Overflow Interrupt Enable bit...1 = Enables the TMR0 interrupt
    INTCONbits.TMR0IF = 0;        // bit2 clear timer 0 interrupt flag
    INTCONbits.GIE = 1;           // bit7 global interrupt enable

    // setup port 0, 1 for ADC
    TRISIObits.TRISIO0 = 1;    // PORT is GPIO0 input...to show the interrupts
    GPIObits.GPIO0 = 1;       // start with GPIO0 inputs low
    
    ANSEL = 0b00000100;  // Set PORT AN0 to analog input AN1 to AN7 digital I/O
    //ANSELbits.ANS2 = 1;  // Set PORT AN2 to analog input AN0 to AN3 digital I/O
    
    /* Init ADC */
    ADCON0=0b00001000;   // select left justify result. ADC port channel 2
    ADCON0bits.ADON = 1;	       // turn on the A2D conversion module
    
    
    iADval = 0;

    while(1)  //endless loop
    {
        ADCON0bits.ADON = 1;
        ADCON0bits.GO_DONE = 1;
        while (ADCON0bits.GO_DONE == 1)
            continue;
        //__delay_us(100);
        
        iADval  = ADRESH;
        if (iADval < 0x18) {
            iLSpeed = (0xff - (iADval << 3)) & 0xff;
            iRSpeed = 0;
        } else if (iADval > 0x20) {
            iRSpeed = iADval;
            iLSpeed = 0;
        } else {
            iLSpeed = 0;
            iRSpeed = 0;
        }
//        eeADH = ADRESH;
//        eeADL = ADRESL;
        __delay_us(2);
    }
}