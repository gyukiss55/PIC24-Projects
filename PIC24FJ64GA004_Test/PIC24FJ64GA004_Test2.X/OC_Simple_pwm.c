/*
 * File:   OC_Simple_pwm.c
 * Author: Apoca
 *
 * Created on 2021. május 3., 12:49
 */
//	_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
	// Disable CLK switch and CLK monitor, OSCO or Fosc/2, HS oscillator,
	// Primary oscillator
//	_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL)

#include <xc.h>
#include "p24fxxxx.h"

#pragma config JTAGEN = OFF
#pragma config GCP = OFF
#pragma config GWRP = OFF
//#pragma config COE = OFF
#pragma config FWDTEN = OFF
#pragma config ICS = PGx1

#pragma config FCKSM = CSDCMD
#pragma config OSCIOFNC = OFF
//#pragma config POSCMOD = HS
//#pragma config FNOSC = PRIPLL
#pragma config FNOSC = FRC


#define TEST1_TRIS      TRISBbits.TRISB5
#define TEST1_PORT      PORTBbits.RB5
#define TEST2_TRIS      TRISBbits.TRISB4
#define TEST2_PORT      PORTBbits.RB4


unsigned char pwmFHtimes;	// PWM frequency hopping times, it is based on PWM cycle

int main (void)
{
	// Disable Watch Dog Timer
	RCONbits.SWDTEN = 0;

	pwmFHtimes = 0;
    
    TEST1_PORT = 1;
    TEST1_TRIS = 0;
    TEST2_PORT = 1;
    TEST2_TRIS = 0;
    
    TEST2_PORT = 0;
    TEST2_PORT = 1;
	
	// PPS
	#ifdef __PIC24FJ64GA002__
		// Unlock Registers
		__builtin_write_OSCCONL(OSCCON & 0xbf);
		// Configure Output Functions *********************
		//RP14 = OC5
		RPOR7bits.RP14R = OC5R;
		// Lock Registers
		__builtin_write_OSCCONL(OSCCON | 0x40);
	#endif

	/*  
	The following code example will set the Output Compare 1 module
	for PWM mode w/o FAULT pin enabled, a 50% duty cycle and 
	a PWM frequency of 52.08 kHz at Fosc = 32 MHz. Timer 2 is selected 
	as the clock for the PWM time base and Timer2 interrupts are enabled.
	 
	Find the Period register value for a desired PWM frequency of 52.08 kHz, 
	where Fosc = 8 MHz with PLL (32 MHz device clock rate) and 
	a Timer2 prescaler setting of 1:1. 
	Tcy = 2/Fosc = 62.5 ns
	PWM Period   =  1/PWM Frequency = 1/52.08 kHz = 19.2 ¦?s
	PWM Period   = (PR2 + 1) * Tcy * (Timer 2 Prescale Value)
	19.2 ¦?s = (PR2 + 1) * 62.5 ns * 1 
	PR2 = 306
	*/
	OC5CONbits.OCM		= 0; 		// Output compare channel is disabled
	OC5R				= 0x0099;	// Initialize Compare Register1 with 50% duty cycle
	OC5RS				= 0x0099;	// Initialize Secondary Compare Register1 with 50% duty cycle
	OC5CONbits.OCSIDL	= 0;		// Output capture will continue to operate in CPU Idle mode
	OC5CONbits.OCFLT	= 0;		// No PWM Fault condition has occurred (this bit is only used when OCM<2:0> = 111)
	OC5CONbits.OCTSEL	= 0;		// Timer2 is the clock source for output Compare
	OC5CONbits.OCM		= 0x6;		// PWM mode on OC, Fault pin disabled
	PR2					= 0x0132;	// Initialize PR2 with 0x0132 = 0d306 as PWM cycle
	IFS0bits.T2IF		= 0;		// Clear Output Compare interrupt flag
	IEC0bits.T2IE		= 1;		// Enable Output Compare interrupts
	T2CONbits.TON		= 1;		// Start Timer2 with assumed settings

	while(1)
	{
		Idle();
	}

	return 0;
}

// Example code for Timer2 ISR: PWM frequency hopping every 5 cycles. 
// Please use oscilloscope to probe on pin 81 of PIC24FJ PIM on 
// Explorer 16 Development Board. You can see the duty cycle will be 
// updated between 50% duty cycle and 75% duty cycle every 5 cycles. 
void __attribute__ ((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
	IFS0bits.T2IF = 0;
    TEST1_PORT = 0;
    TEST1_PORT = 1;
	
	if (++pwmFHtimes == 5) {	// every 5 cycles for update
		pwmFHtimes = 0;
		// update the duty cycle for PWM
		if (OC5RS == 0x0099)
			OC5RS = 0x00E5;	// 75% duty cycle
		else
			OC5RS = 0x0099;	// 50% duty cycle
	}
}
