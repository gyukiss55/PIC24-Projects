/*
 * File:   main.c
 * Author: Apoca
 *
 * Created on 2021. április 20., 17:52
 */
// To compile:
//
//    xc8 --chip=12F675 main.c
//
 
#include <xc.h>
 
#pragma config FOSC=INTRCIO,WDTE=OFF,MCLRE=OFF,BOREN=OFF
 
void main(void)
{
    TRISIO = 0b11011111; // Make pin GP5 a digital output
     
    while(1)
    {
        GP5 = 1;         // Set pin GP5 high
        _delay(10000);  // 0.5 second delay
        GP5 = 0;         // Set pin GP5 low
        _delay(200000);  // 0.5 second delay
    }
}