/*
 * File:   main.c
 * Author: Apoca
 *
 * Created on 2021. április 25., 9:45
 */


#include <xc.h>
#include <PIC18F4520.h>

#define _XTAL_FREQ 8000000
#pragma config MCLRE=OFF,BOREN=OFF

unsigned char iCnt = 0;
 
void main(void)
{
    
    TRISBbits.TRISB5 = 0;    // PORT is RB5 output...to show the interrupts
    PORTBbits.RB5 = 0;       // start with RB5 outputs low
    TRISBbits.TRISB4 = 0;    // PORT is RB4 output...to show the interrupts
    PORTBbits.RB4 = 0;       // start with RB4 outputs low
    
    TRISB = 0;
    PORTB = 0;

    while(1)  //endless loop
    {
        ++iCnt;
        PORTB = iCnt;
       __delay_us(2000);
    }
}