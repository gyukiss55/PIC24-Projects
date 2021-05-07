/**********************************************************************
* ?2008 Microchip Technology Inc.
*
* FileName:        SPI_Slave.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC24Fxxxx
* Compiler:        MPLAB?C30 v3.20 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all 
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your 
* behalf, exclusively with Microchip's proprietary products.  Your 
* acceptance and/or use of this code constitutes agreement to the 
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT 
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS 
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH 
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE 
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, 
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR 
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER 
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP 
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Albert Z.		3/27/09		first release
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This code example is used for standard buffer mode, 
* support PIC24FJ64GA002.
* 16-bit 28P Starter Demo Board, 
* #16:SCK1, #15:SDI1, #21:SDO1, #22:/SS1
**********************************************************************/
#include "p24fxxxx.h"


#ifdef __PIC24FJ64GA002__	//Defined by MPLAB when using 24FJ64GA004 device
#pragma config  JTAGEN = OFF 
#pragma config  GCP = OFF
#pragma config  GWRP = OFF
//#pragma config  COE = OFF
#pragma config  FWDTEN = OFF
#pragma config  ICS = PGx1
#pragma config  FCKSM = CSDCMD
#pragma config  OSCIOFNC = OFF
//#pragma config  POSCMD = PRI
#pragma config  FNOSC = FRCPLL
#pragma config  I2C1SEL = SEC
#pragma config  IOL1WAY = ON
#endif


// RB2 as SS for SPI slave MCU
#define TEST_TRIS       TRISBbits.TRISB5
#define TEST_PORT       PORTBbits.RB5
#define SPI_SS_TRIS     TRISBbits.TRISB8
#define SPI_SS_PORT     PORTBbits.RB8
#define DAC_EN_TRIS     TRISBbits.TRISB9
#define DAC_EN_PORT     PORTBbits.RB9

unsigned short spiBufT[5];		// SPI buffer for transmission
unsigned short spiBufR[5];	// SPI buffer for Receiving

unsigned char spiCount=0;		// variable for how many data has been received from SPI bus

unsigned short v1 = 0x400;
unsigned short v2 = 0;

const short sinTable[] = { 0, 12, 25, 37, 50, 62, 75, 87, 100, 113, 125, 138, 150, 163, 175, 188, 200, 213, 225, 238, 250, 263, 275, 288, 300, 312, 325, 337, 350, 362, 374, 387, 399, 411, 424, 436, 448, 460, 473, 485, 497, 509, 521, 534, 546, 558, 570, 582, 594, 606, 618, 630, 642, 654, 666, 678, 689, 701, 713, 725, 737, 748, 760, 772, 783, 795, 806, 818, 829, 841, 852, 864, 875, 886, 898, 909, 920, 932, 943, 954, 965, 976, 987, 998, 1009, 1020, 1031, 1042, 1052, 1063, 1074, 1085, 1095, 1106, 1116, 1127, 1137, 1148, 1158, 1168, 1179, 1189, 1199, 1209, 1219, 1230, 1240, 1250, 1259, 1269, 1279, 1289, 1299, 1308, 1318, 1328, 1337, 1347, 1356, 1366, 1375, 1384, 1393, 1403, 1412, 1421, 1430, 1439, 1448, 1457, 1465, 1474, 1483, 1491, 1500, 1509, 1517, 1525, 1534, 1542, 1550, 1558, 1567, 1575, 1583, 1591, 1598, 1606, 1614, 1622, 1629, 1637, 1644, 1652, 1659, 1667, 1674, 1681, 1688, 1695, 1702, 1709, 1716, 1723, 1730, 1736, 1743, 1750, 1756, 1763, 1769, 1775, 1781, 1788, 1794, 1800, 1806, 1812, 1817, 1823, 1829, 1834, 1840, 1845, 1851, 1856, 1861, 1867, 1872, 1877, 1882, 1887, 1892, 1896, 1901, 1906, 1910, 1915, 1919, 1924, 1928, 1932, 1936, 1940, 1944, 1948, 1952, 1956, 1959, 1963, 1966, 1970, 1973, 1977, 1980, 1983, 1986, 1989, 1992, 1995, 1998, 2000, 2003, 2006, 2008, 2011, 2013, 2015, 2017, 2019, 2021, 2023, 2025, 2027, 2029, 2031, 2032, 2034, 2035, 2036, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2044, 2045, 2046, 2046, 2047, 2047, 2047, 2047, 2047};

short GetSinValue (int ix)
{
    short v1 = 0;
    short ix2;
    ix2 = ix & 255;
    switch (ix >> 8) {
        case 0:
            v1 = sinTable[ix2] + 0x800;
            break;
        case 1:
            v1 = sinTable[255 - ix2] + 0x800;
            break;
        case 2:
            v1 = 0x800 - sinTable[ix2];
            break;
        default:
            v1 = 0x800 - sinTable[255 - ix2];
            break;
    }
    return v1;
}

void SPI1Init(void)
{
    //config SPI1
    SPI1STATbits.SPIEN 		= 0;	// disable SPI port
    SPI1STATbits.SPISIDL 	= 0; 	// Continue module operation in Idle mode
    
    SPI1BUF 				= 0;   	// clear SPI buffer
    
    IFS0bits.SPI1IF 		= 0;	// clear interrupt flag
    IEC0bits.SPI1IE 		= 0;	// enable interrupt
    
    SPI1CON1bits.DISSCK		= 0;	// Internal SPIx clock is enabled
    SPI1CON1bits.DISSDO		= 0;	// SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 	= 1;	// set in 16-bit mode, clear in 8-bit mode
    SPI1CON1bits.SMP		= 0;	// SMP must be cleared when SPIx is used in Slave mode
    SPI1CON1bits.CKP 		= 0;	// CKP and CKE is subject to change ...
    SPI1CON1bits.CKE 		= 1;	// ... based on your communication mode.
    SPI1CON1bits.SSEN		= 0;	// SSx pin is used for Slave mode
	SPI1CON1bits.MSTEN 		= 1; 	// 1 =  Master mode; 0 =  Slave mode
	SPI1CON1bits.SPRE 		= 7; 	// Secondary Prescaler = 4:1
	SPI1CON1bits.PPRE 		= 3; 	// Primary Prescaler = 4:1
    
    SPI1CON2 				= 0;	// non-framed mode
    
    SPI1STATbits.SPIEN 		= 1; 	// enable SPI port, clear status
}

unsigned short writeSPI1( unsigned short data )
{
//    int i;
    SPI_SS_PORT = 0;
    SPI1BUF = data;					// write to buffer for TX
    while(!SPI1STATbits.SPIRBF);	// wait for transfer to complete
    SPI_SS_PORT = 1;
    return SPI1BUF;    				// read the received value
}//writeSPI1

int main(void)
{
    unsigned int i;
    unsigned int ix = 0;
    unsigned int ix3 = 0;
	// Disable Watch Dog Timer
	RCONbits.SWDTEN = 0;
    
	// remap pins before intialising SPI1
#if defined (__PIC24FJ64GA002__)
	// Unlock Registers
	__builtin_write_OSCCONL(OSCCON & 0xbf);
	// Configure Input Functions **********************
	// SDI1 = RP6
	RPINR20bits.SDI1R = 6;

	// Configure Output Functions *********************
	//RP10 = SDO1, SCK1 = RP7, SS1 = RP11
	RPOR3bits.RP7R = 7;	// SDO1
	RPOR3bits.RP6R = 8;     //SCLK1
	//RPOR4bits.RP8R = 9;       //SS1  dummy
	// Lock Registers
	__builtin_write_OSCCONL(OSCCON | 0x40);

	AD1PCFGbits.PCFG12 = 1;	// config RB12 in digital mode
	ODCBbits.ODB12 = 0;		// disable open-drain mode
	TRISBbits.TRISB12 = 0;	// GB12 as output
#endif
    
    TEST_PORT = 1;
    TEST_TRIS = 0;


	SPI1Init();
    
    SPI_SS_PORT				= 1;	// 
	SPI_SS_TRIS				= 0;	// set SS as output
    
    DAC_EN_PORT				= 1;	// 
	DAC_EN_TRIS				= 0;	// set SS as output
	
    
    
    while (1) {
        
        
        v1 = GetSinValue (ix);
        v2 = GetSinValue (ix3);
        ix++;
        if (ix >= 4 * 256)
            ix = 0;
        ix3+=5;
        if (ix3 >= 4 * 256)
            ix3 = 0;
        
//        spiBufT[0] = v1 | 0x3000;
//        spiBufT[1] = v2 | 0xB000;

    //	spiBufR[0]	= writeSPI1(0x3555);
        
    //	spiBufR[1]	= writeSPI1(0xBFA0);
        __builtin_btg((unsigned int *)&LATB, 5);
        
        writeSPI1 (v1 | 0x3000);
        writeSPI1 (v2 | 0xB000);
        
        __builtin_btg((unsigned int *)&LATB, 5);
        
        DAC_EN_PORT = 0;
        for (i = 0; i < 10; ++i)
            asm ("NOP");       
        DAC_EN_PORT = 1;
        

	} 
	
    
 }


void __attribute__((interrupt, no_auto_psv))  _SPI1Interrupt(void)
{
	IFS0bits.SPI1IF = 0;
   
	if (SPI1STATbits.SPIROV) {
		// error
		SPI1STATbits.SPIROV	= 0;			// clear overflow
		spiBufR[3]	= SPI1BUF;				// read data, spiBufR[3] is for junk
	} else if (!SPI1STATbits.SPIRBF) {
		// error
	} else {
		if (spiCount==0) {
			spiBufR[0]	= SPI1BUF;		// read data 1
			
			if (spiBufR[0] == 0x00ac)	// this is the header data
				spiCount++;				// continue receiving next; otherwise wait for the header 
			
			spiBufT[5] = spiBufR[0] + 1;		// loop with data + 1
		} else {
			spiBufR[spiCount] = SPI1BUF;
			spiBufT[5] = spiBufR[spiCount++] + 1;	// loop with data + 1
		}
		
		while (SPI1STATbits.SPITBF);
		SPI1BUF = spiBufT[5];				// send back
	}	
}
 
