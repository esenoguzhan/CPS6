#include "lab03.h"

#include <xc.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL
#include <libpic30.h>

#include "types.h"
#include "lcd.h"
#include "led.h"

/*
 * DAC code
 */

#define DAC_CS_TRIS TRISDbits.TRISD8
#define DAC_SDI_TRIS TRISBbits.TRISB10
#define DAC_SCK_TRIS TRISBbits.TRISB11
#define DAC_LDAC_TRIS TRISBbits.TRISB13
    
#define DAC_CS_PORT PORTDbits.RD8
#define DAC_SDI_PORT PORTBbits.RB10
#define DAC_SCK_PORT PORTBbits.RB11
#define DAC_LDAC_PORT PORTBbits.RB13

#define DAC_SDI_AD1CFG AD1PCFGLbits.PCFG10
#define DAC_SCK_AD1CFG AD1PCFGLbits.PCFG11
#define DAC_LDAC_AD1CFG AD1PCFGLbits.PCFG13

#define DAC_SDI_AD2CFG AD2PCFGLbits.PCFG10
#define DAC_SCK_AD2CFG AD2PCFGLbits.PCFG11
#define DAC_LDAC_AD2CFG AD2PCFGLbits.PCFG13

void dac_initialize()
{
    // set AN10, AN11 AN13 to digital mode
    // this means AN10 will become RB10, AN11->RB11, AN13->RB13
    // see datasheet 11.3
    SETBIT(DAC_SDI_AD1CFG);
	SETBIT(DAC_SCK_AD1CFG);
	SETBIT(DAC_LDAC_AD1CFG);

	SETBIT(DAC_SDI_AD2CFG);
	SETBIT(DAC_SCK_AD2CFG);
	SETBIT(DAC_LDAC_AD2CFG);
   
    // set RD8, RB10, RB11, RB13 as output pins

	CLEARBIT(DAC_SDI_TRIS);
	CLEARBIT(DAC_SCK_TRIS);
	CLEARBIT(DAC_LDAC_TRIS);
    
    // set default state: CS=??, SCK=??, SDI=??, LDAC=??

	
    
}

/*
 * Timer code
 */

#define FCY_EXT   32768UL

#define TCKPS_1   0x00
#define TCKPS_8   0x01
#define TCKPS_64  0x02
#define TCKPS_256 0x03

void timer_initialize()
{
    // Enable RTC Oscillator -> this effectively does OSCCONbits.LPOSCEN = 1
    // but the OSCCON register is lock protected. That means you would have to 
    // write a specific sequence of numbers to the register OSCCONL. After that 
    // the write access to OSCCONL will be enabled for one instruction cycle.
    // The function __builtin_write_OSCCONL(val) does the unlocking sequence and
    // afterwards writes the value val to that register. (OSCCONL represents the
    // lower 8 bits of the register OSCCON)
    __builtin_write_OSCCONL(OSCCONL | 2);
    // configure timer
	OSCCONbits.LPOSCEN = 1
	T1CONbits.TON = 0; // Disable Timer
	T1CONbits.TCS = 1; // Select external clock
	T1CONbits.TSYNC = 0; // Disable Synchronization
	T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
	TMR1 = 0x00; // Clear timer register
	PR1 = FCY_EXT / 2 ; // Load the period value
	IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	IEC0bits.T1IE = 1; // Enable Timer1 interrupt
	T1CONbits.TON = 1; // Start Timer

	OSCCONbits.LPOSCEN = 1
	T2CONbits.TON = 0; // Disable Timer
	T2CONbits.TCS = 1; // Select external clock
	T2CONbits.TSYNC = 0; // Disable Synchronization
	T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
	TMR2 = 0x00; // Clear timer register
	PR2 = FCY; // Load the period value
	IPC0bits.T2IP = 0x03; // Set Timer1 Interrupt Priority Level
	IFS0bits.T2IF = 0; // Clear Timer1 Interrupt Flag
	IEC0bits.T2IE = 1; // Enable Timer1 interrupt
	T2CONbits.TON = 1; // Start Timer


	OSCCONbits.LPOSCEN = 1
	T3CONbits.TON = 0; // Disable Timer
	T3CONbits.TCS = 1; // Select external clock
	T3CONbits.TSYNC = 0; // Disable Synchronization
	T3CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
	TMR3 = 0x00; // Clear timer register
	PR3 = 2 * FCY_EXT; // Load the period value
	IPC0bits.T3IP = 0x05; // Set Timer1 Interrupt Priority Level
	IFS0bits.T3IF = 0; // Clear Timer1 Interrupt Flag
	IEC0bits.T3IE = 1; // Enable Timer1 interrupt
	T3CONbits.TON = 1; // Start Timer
}

// interrupt service routine?

/*
 * main loop
 */

void main_loop()
{
    // print assignment information
    lcd_printf("Lab03: DAC");
    lcd_locate(0, 1);
    lcd_printf("Group: 06");
    
    while(TRUE)
    {
        // main loop code
    }
}
