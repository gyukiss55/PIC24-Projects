/*
 * File:   main.c
 * Author: Apoca
 * PIC18F24K40_I2c_Test
 *
 * Created on 2021. április 10., 9:35
 */



//#include "mcc_generated_files/mcc.h"
#include"xc.h"
#include"pic18.h"
//#include "18f24k40.cgen.inc"
unsigned char Digits[4]={0,0,0,0}; 
unsigned char TempData;
#define SDA RC4 // Data pin for i2c
#define SCK RC3 // Clock pin for i2c
#define SDA_DIR TRISC4 // Data pin direction
#define SCK_DIR TRISC3 // Clock pin direction

// Define i2c speed
#define I2C_SPEED 100 // kbps
#pragma config WDTE = OFF


// Function Purpose: I2C_Start sends start bit sequence
void I2C_Start(void)
{
    SSP1CON2bits.SEN=1;
 
    while(!PIR3bits.SSPIF) // Wait for it to complete
        ;
    PIR3bits.SSPIF = 0; // Clear the flag bit
}


// Function Purpose: I2C_ReStart sends start bit sequence
void I2C_ReStart(void)
{
    SSP1CON2bits.RSEN = 1; // Send Restart bit
    while(!PIR3bits.SSPIF) // Wait for it to complete
        ;
    PIR3bits.SSPIF = 0; 
}


//Function : I2C_Stop sends stop bit sequence
void I2C_Stop(void)
{
    SSP1CON2bits.PEN = 1; // Send stop bit
    while(!PIR3bits.SSPIF) // Wait for it to complete
        ;
    PIR3bits.SSPIF = 0; 
}



//Function : I2C_Send_ACK sends ACK bit sequence
void I2C_Send_ACK(void)
{
    SSP1CON2bits.ACKDT = 0; // 0 means ACK
    SSP1CON2bits.ACKEN = 1; // Send ACKDT value
    while(!PIR3bits.SSPIF) // Wait for it to complete
        ;
    PIR3bits.SSPIF = 0; 
}


//Function : I2C_Send_NACK sends NACK bit sequence
void I2C_Send_NACK(void)
{
    SSP1CON2bits.ACKDT = 1; // 0 means ACK
    SSP1CON2bits.ACKEN = 1; // Send ACKDT value
    while(!PIR3bits.SSPIF) // Wait for it to complete
        ;
    PIR3bits.SSPIF = 0; 
}


// Function Purpose: I2C_Write_Byte transfers one byte
unsigned char I2C_Write_Byte(unsigned char Byte)
{
    SSP1BUF = Byte; // Send Byte value
    while(!PIR3bits.SSPIF) // Wait for it to complete
       ;
    PIR3bits.SSPIF = 0; 

    return SSP1CON2bits.ACKSTAT; // Return ACK/NACK from slave
}


// Function Purpose: I2C_Read_Byte reads one byte
unsigned char I2C_Read_Byte(void)
{
    SSP1CON2bits.RCEN = 1; // Enable reception of 8 bits
    while(!PIR3bits.SSPIF) // Wait for it to complete
        ;
    PIR3bits.SSPIF = 0; 
 
    return SSP1BUF ; // Return received byte
}




void main(void)
{
    // Initialize the device
//    SYSTEM_Initialize();

    
    
     
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    unsigned char RxByte = 0;

        

    I2C_Start(); // Send start bit on i2c
    I2C_Write_Byte(0xA0); // Send 0xA0 on i2c
    RxByte = I2C_Read_Byte(); // Read value from i2c
    I2C_Send_ACK(); // Send ACK bit on i2c
    I2C_Stop(); // Send stop bit on i2c
    
        
    while (1)
    {
      
        
    }
}