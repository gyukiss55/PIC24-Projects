/*
 * File:   main.c
 * Author: Apoca
 *
 * Created on 2021. április 9., 18:14
 */


#include <xc.h>

#pragma config WDTE = OFF
#define _XTAL_FREQ 2000000

void main(void) {
    TRISIObits.TRISIO2 = 0;
    while (1){
        GPIObits.GP2 = 1;
        __delay_ms (500);
        GPIObits.GP2 = 0;
        __delay_ms (500);
    }
    return;
}
