/*
 * File:   main_2.c
 * Author: Apoca
 *
 * Created on 2021. április 9., 15:56
 */


#include <xc.h>

#define config WDTE = OFF
#define _XTAL_FREQ 4000000

void main(void) {
    TRISAbits.TRISA2=0;
    while (1){
        LATAbits.LATA2 = 1;
        __delay_ms (500);
        LATAbits.LATA2 = 0;
        __delay_ms (500);
    }
    return;
}
