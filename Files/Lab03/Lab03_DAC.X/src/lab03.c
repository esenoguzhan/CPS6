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
    CLEARBIT(DAC_CS_TRIS)
    CLEARBIT(DAC_SDI_TRIS);
    CLEARBIT(DAC_SCK_TRIS);
    CLEARBIT(DAC_LDAC_TRIS);   
    
    // set default state: CS=1, SCK=0, SDI=0, LDAC=1 (as per MCP4822 datasheet Section 5)
    SETBIT(DAC_CS_PORT)
    CLEARBIT(DAC_SDI_PORT);
    CLEARBIT(DAC_SCK_PORT);
    SETBIT(DAC_LDAC_PORT);
}

void dac_output(uint16_t value)
{
    // MCP4822 format (Register 5-1, Figure 5-1):
    // Bits 15-12: Configuration (A/B=0, BUF=0, GA=1, SHDN=1)
    // Bits 11-0:  12-bit data
    uint16_t command = 0b0011000000000000 | (value & 0x0FFF);

    CLEARBIT(DAC_CS_PORT); // Clear CS to start transmission (as per Table 7)
    Nop();
    
    // Send 16 bits MSB first
    for (int i = 15; i >= 0; i--)
    {
        // Set SDI according to current bit
        if (command & (1 << i)){
            SETBIT(DAC_SDI_PORT);
        }
        else{
            CLEARBIT(DAC_SDI_PORT);
        }
        Nop(); // Insert Nop between write operations
        
        // Toggle SCK (rising edge)
        SETBIT(DAC_SCK_PORT);
        Nop(); // As per Figure 5-1 timing requirements
        CLEARBIT(DAC_SCK_PORT);
        Nop(); // As per Figure 5-1 timing requirements
    }
    
    // Set CS to end transmission
    SETBIT(DAC_CS_PORT); 
    Nop();
    
    // Clear SDI
    CLEARBIT(DAC_SDI_PORT);
    Nop();

    // Toggle LDAC to latch data to output (as per Section 1 AC Characteristics)
    CLEARBIT(DAC_LDAC_PORT);
    Nop(); Nop();
    SETBIT(DAC_LDAC_PORT);
}

/*
 * Timer code
 */

#define FCY_EXT   32768UL

#define TCKPS_1   0x00
#define TCKPS_8   0x01
#define TCKPS_64  0x02
#define TCKPS_256 0x03

volatile int delay_elapsed = 0;

// interrupt service routine?
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    delay_elapsed = 1;
    IFS0bits.T1IF = 0;
}

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
    T1CON = 0;              // Clear Timer1 config
    TMR1 = 0;               // Clear counter
    T1CONbits.TCKPS = TCKPS_64; // Set prescaler
    IFS0bits.T1IF = 0;      // Clear interrupt flag
    IEC0bits.T1IE = 1;      // Enable interrupt
    
}


void timer_start_ms(uint16_t milliseconds)
{
    T1CONbits.TON = 0;      // Stop Timer1
    TMR1 = 0;               // Reset counter
    PR1 = (FCY / 64 / 1000) * milliseconds;
    delay_elapsed = 0;
    IFS0bits.T1IF = 0;      // Clear interrupt flag
    T1CONbits.TON = 1;      // Start Timer1
}

/*
 * main loop
 */

void main_loop()
{
    // print assignment information
    lcd_printf("Lab03: DAC");
    lcd_locate(0, 1);
    lcd_printf("Group: 6");
    
    dac_initialize();
    timer_initialize();
    
    while(TRUE)
    {
        // Generate different voltage levels with proper timing
        dac_output(1000);      // ~1.0V output (DAC CHAN A, Pin 5 of J3)
        timer_start_ms(500);
        while (!delay_elapsed);

        dac_output(2500);      // ~2.5V
        timer_start_ms(2000);
        while (!delay_elapsed);

        dac_output(3500);      // ~3.5V
        timer_start_ms(1000);
        while (!delay_elapsed);

        TOGGLELED(LED1_PORT);         // Toggle LED1
    }
}
