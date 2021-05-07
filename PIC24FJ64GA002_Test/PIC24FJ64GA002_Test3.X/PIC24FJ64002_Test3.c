/*
 * File:   main.c
 * Author: Apoca
 *
 * Created on 2021. április 25., 15:58
 */

#include <xc.h>
#include <P24FJ64GA002.h>


#pragma config GWRP = OFF
#pragma config FWDTEN = OFF

//#pragma config FNOSC0 = OFF
//#pragma config FNOSC1 = OFF
//#pragma config FNOSC2 = OFF

//#pragma config FCKSM0 = ON
//#pragma config FCKSM1 = ON
//#pragma config OSCIOFCN = OFF

//#pragma config IESO = OFF

//#define _XTAL_FREQ 8000000

#define DELAYMACRO
unsigned char iCnt = 0;

int TestBlinkLed ()
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
    }
    
    return 0;
  
}


// RB2 as SS for SPI slave MCU
#define SPI_SS_TRIS      TRISBbits.TRISB2
#define SPI_SS_PORT      PORTBbits.RB2

#define TEST1_TRIS      TRISBbits.TRISB3
#define TEST1_PORT      PORTBbits.RB3

#define DAC_CL_TRIS      TRISBbits.TRISB6
#define DAC_CL_PORT      PORTBbits.RB6

#define DAC_DO_TRIS      TRISBbits.TRISB7
#define DAC_DO_PORT      PORTBbits.RB7

#define DAC_SS_TRIS      TRISBbits.TRISB8
#define DAC_SS_PORT      PORTBbits.RB8

#define DAC_EN_TRIS      TRISBbits.TRISB9
#define DAC_EN_PORT      PORTBbits.RB9

unsigned short spiBufT[]={0x00ff,0x5555,0xffff,0xffff};	// for demo only
unsigned short spiBufR[]={0,0,0,0};	// SPI buffer for Receiving

void SPI1Init(void)
{

    //config SPI1
    SPI1STATbits.SPIEN 		= 0;	// disable SPI port
    SPI1STATbits.SPISIDL 	= 0; 	// Continue module operation in Idle mode
    
    SPI1BUF 				= 0;   	// clear SPI buffer
    
    IFS0bits.SPI1IF 		= 0;	// clear interrupt flag
    IEC0bits.SPI1IE 		= 0;	// disable interrupt
    
    SPI1CON1bits.DISSCK		= 0;	// Internal SPIx clock is enabled
    SPI1CON1bits.DISSDO		= 0;	// SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 	= 1;	// set in 16-bit mode, clear in 8-bit mode
    SPI1CON1bits.SMP		= 0;	// Input data sampled at middle of data output time
    SPI1CON1bits.CKP 		= 1;	// CKP and CKE is subject to change ...
    SPI1CON1bits.CKE 		= 0;	// ... based on your communication mode.
	SPI1CON1bits.MSTEN 		= 1; 	// 1 =  Master mode; 0 =  Slave mode
	SPI1CON1bits.SPRE 		= 1; 	// Secondary Prescaler = 4:1
	SPI1CON1bits.PPRE 		= 1; 	// Primary Prescaler = 4:1
	
    SPI1CON2 				= 0;	// non-framed mode
    
	SPI_SS_PORT				= 1;	// 
	SPI_SS_TRIS				= 0;	// set SS as output
	
    SPI1STATbits.SPIEN 		= 1; 	// enable SPI port, clear status
}

unsigned short writeSPI1( unsigned short data )
{
    SPI1BUF = data;					// write to buffer for TX
    while(!SPI1STATbits.SPIRBF);	// wait for transfer to complete
    return SPI1BUF;    				// read the received value
}//writeSPI1

int TestSPI1 (void)
{
	unsigned short i;
	
	// Disable Watch Dog Timer
	RCONbits.SWDTEN = 0;
	// for LED
	SPI1Init();
	
    while (1) {
	    for (i=0; i<0xffff; i++);	// a simple delay
		
		SPI_SS_PORT	= 0;
		spiBufR[0]	= writeSPI1(spiBufT[0]);
		spiBufR[1]	= writeSPI1(spiBufT[1]);
		spiBufR[2]	= writeSPI1(spiBufT[2]);
		spiBufR[3]	= writeSPI1(0);
		SPI_SS_PORT = 1;

	    __builtin_btg((unsigned int *)&LATA, 6);
	} 

	return 0;
}

void SPI2Init(void)
{
    //config SPI2
    SPI2STATbits.SPIEN 		= 0;	// disable SPI port
    SPI2STATbits.SPISIDL 	= 0; 	// Continue module operation in Idle mode
    
    SPI2BUF 				= 0;   	// clear SPI buffer
    
    IFS2bits.SPI2IF 		= 0;	// clear interrupt flag
    IEC2bits.SPI2IE 		= 0;	// disable interrupt
    
    SPI2CON1bits.DISSCK		= 0;	// Internal SPIx clock is enabled
    SPI2CON1bits.DISSDO		= 0;	// SDOx pin is controlled by the module
    SPI2CON1bits.MODE16 	= 1;	// set in 16-bit mode, clear in 8-bit mode
    SPI2CON1bits.SMP		= 0;	// Input data sampled at middle of data output time
    SPI2CON1bits.CKP 		= 1;	// CKP and CKE is subject to change ...
    SPI2CON1bits.CKE 		= 0;	// ... based on your communication mode.
	SPI2CON1bits.MSTEN 		= 1; 	// 1 =  Master mode; 0 =  Slave mode
	SPI2CON1bits.SPRE 		= 1; 	// Secondary Prescaler = 4:1
	SPI2CON1bits.PPRE 		= 1; 	// Primary Prescaler = 4:1
	
    SPI1CON2 				= 0;	// non-framed mode
    
	SPI_SS_PORT				= 1;	// 
	SPI_SS_TRIS				= 0;	// set SS as output
	
    SPI2STATbits.SPIEN 		= 1; 	// enable SPI port, clear status
}

unsigned short writeSPI2( unsigned short data )
{
    SPI2BUF = data;					// write to buffer for TX
    while(!SPI2STATbits.SPIRBF);	// wait for transfer to complete
    return SPI2BUF;    				// read the received value
}//writeSPI1

int TestSPI2 (void)
{
	unsigned short i;
	
	// Disable Watch Dog Timer
	RCONbits.SWDTEN = 0;
	// for LED
	SPI2Init();
	
    while (1) {
	    for (i=0; i<0xffff; i++);	// a simple delay
		
		SPI_SS_PORT	= 0;
		spiBufR[0]	= writeSPI2(spiBufT[0]);
		spiBufR[1]	= writeSPI2(spiBufT[1]);
		spiBufR[2]	= writeSPI2(spiBufT[2]);
		spiBufR[3]	= writeSPI2(0);
		SPI_SS_PORT = 1;

	    __builtin_btg((unsigned int *)&LATA, 6);
	} 

	return 0;
}

void swSPIDelay ()
{
//    int i;
//    for (i=0; i<0xfff; i++);	// a simple delay
}

void swSPIWrite (unsigned short w1, unsigned short w2)
{
    int i;
    DAC_SS_PORT = 0;
    DELAYMACRO;

    for (i = 0; i < 16; ++i)
    {
        if (w1 & 0x8000)
            DAC_DO_PORT = 1;
        else
            DAC_DO_PORT = 0;
        w1 = w1 << 1;
        DELAYMACRO;
        DAC_CL_PORT = 1;
        DELAYMACRO;
        DAC_CL_PORT = 0;
        DELAYMACRO;
    }
    
    DAC_SS_PORT = 1;
    DELAYMACRO;
//    DAC_EN_PORT = 0;
//    swSPIDelay ();
//    DAC_EN_PORT = 1;
//    swSPIDelay ();
    
    DAC_SS_PORT = 0;
    DELAYMACRO;
    for (i = 0; i < 16; ++i)
    {
        if (w2 & 0x8000)
            DAC_DO_PORT = 1;
        else
            DAC_DO_PORT = 0;
        w2 = w2 << 1;
        DELAYMACRO;
        DAC_CL_PORT = 1;
        DELAYMACRO;
        DAC_CL_PORT = 0;
        DELAYMACRO;
    }
    DAC_SS_PORT = 1;
    DELAYMACRO;
    DAC_EN_PORT = 0;
    DELAYMACRO;
    DAC_EN_PORT = 1;
    DELAYMACRO;
}

#define ADCH0 0x3000
#define ADCH1 0xB000

void TestSPIsw ()
{
    unsigned short dac_v1 = 0x800;
    unsigned short dac_v2 = 0;
 /*               
    OSCCONbits.COSC0 = 0;
    OSCCONbits.COSC1 = 0;
    OSCCONbits.COSC2 = 0;
    
    OSCCONbits.NOSC0 = 0;
    OSCCONbits.NOSC1 = 0;
    OSCCONbits.NOSC2 = 0;
    OSCCONbits.SOSCEN = 0;
    
    CLKDIVbits.DOZEN = 0;
*/            
            
    DAC_CL_PORT = 1;       // start with RB5 outputs low
    DAC_CL_TRIS = 0;    // CLK
    DAC_DO_PORT = 1;       // start with RB4 outputs low
    DAC_DO_TRIS = 0;    // DATA
    DAC_SS_PORT = 1;       // start with RB4 outputs high
    DAC_SS_TRIS = 0;    // CS
    DAC_EN_PORT = 1;       // start with RB4 outputs high
    DAC_EN_TRIS = 0;    // DACEN

	TEST1_PORT = 1;	// 
	TEST1_TRIS = 0;	// set SS as output
    
    TEST1_PORT = 0;
    TEST1_PORT = 1;

    while (1) {
        dac_v2 = (0xFFF - dac_v1) & 0xFFF;
        swSPIWrite (dac_v1 | ADCH0, dac_v2 | ADCH1);
        dac_v1+=0x1;
        if (dac_v1 >= 0x1000)
            dac_v1 = 0x000;
    }
   
}


void TestOsc ()
{
    
#define _ASM_PART_
    
#if defined (_ASM_PART_)   
//Place the new oscillator selection in W0
//OSCCONH (high byte) Unlock Sequence
    asm("MOV #OSCCONH,w1");
    asm("MOV #0x78, w2");
    asm("MOV #0x9A, w3");
    asm("MOV.b w2, [w1]");
    asm("MOV.b w3, [w1]");
//Set new oscillator selection

    asm("MOV.b WREG, OSCCONH");

//OSCCONL (low byte) unlock sequence
    asm("MOV #OSCCONL,w1");
    asm("MOV.b #0x01, w0");
    asm("MOV #0x46, w2");
    asm("MOV #0x57, w3");
    asm("MOV.b w2, [w1]");
    asm("MOV.b w3, [w1]");
//Start oscillator switch operation
asm("MOV.b w0, [w1]");

#endif
    
  	TEST1_PORT = 1;	// 
	TEST1_TRIS = 0;	// set SS as output
    while (1) {
        TEST1_PORT = 0;
        asm("NOP");
        TEST1_PORT = 1;
    }
}

int main(void)
{
 //   TestBlinkLed ();
 //   TestSPI2 ();
 //    TestSPIsw ();
   TestOsc ();
    return 0;
}