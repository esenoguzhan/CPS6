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
    SETBIT(DAC_SDI_AD1CFG);
    SETBIT(DAC_SCK_AD1CFG);
    SETBIT(DAC_LDAC_AD1CFG);

    SETBIT(DAC_SDI_AD2CFG);
    SETBIT(DAC_SCK_AD2CFG);
    SETBIT(DAC_LDAC_AD2CFG);

    // set RD8, RB10, RB11, RB13 as output pins
    CLEARBIT(DAC_CS_TRIS);
    CLEARBIT(DAC_SDI_TRIS);
    CLEARBIT(DAC_SCK_TRIS);
    CLEARBIT(DAC_LDAC_TRIS);

    // set default state: CS=1, SCK=0, SDI=0, LDAC=1
    SETBIT(DAC_CS_PORT);
    CLEARBIT(DAC_SCK_PORT);
    CLEARBIT(DAC_SDI_PORT);
    SETBIT(DAC_LDAC_PORT);
}

void dac_output(uint16_t value)
{
    // Using gain=2x for voltages up to 4.096V
    uint16_t command = 0x1000 | (value & 0x0FFF); // DAC A, gain=2x, active

    CLEARBIT(DAC_CS_PORT);
    Nop();

    int i = 0;
    for (i = 15; i >= 0; i--) {
        DAC_SDI_PORT = (command >> i) & 0x1;
        Nop();
        SETBIT(DAC_SCK_PORT);
        Nop();
        CLEARBIT(DAC_SCK_PORT);
        Nop();
    }

    SETBIT(DAC_CS_PORT);
    Nop();
    CLEARBIT(DAC_LDAC_PORT);
    Nop();
    Nop();
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

volatile int timer_flag = 0;

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    timer_flag = 1;
    IFS0bits.T1IF = 0; // Clear interrupt flag
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
    __builtin_write_OSCCONL(OSCCONL | 2); // Enable RTC oscillator

    T1CON = 0; // Clear Timer1 configuration
    T1CONbits.TCS = 1; // Use external clock (32.768kHz)
    T1CONbits.TCKPS = TCKPS_64; // 1:64 prescaler
    PR1 = 512; // 32768/64 = 512Hz -> 512/512 = 1Hz (1s period)
    TMR1 = 0; // Clear timer
    IFS0bits.T1IF = 0; // Clear interrupt flag
    IEC0bits.T1IE = 1; // Enable interrupt
    T1CONbits.TON = 1; // Start timer
}

void delay_ms(uint16_t ms)
{
    // Adjust timer for requested delay
    if (ms == 500) {
        PR1 = 256; // 0.5s period
    }
    else if (ms == 2000) {
        PR1 = 1024; // 2s period
    }
    else {
        PR1 = 512; // 1s period
    }

    TMR1 = 0; // Reset timer
    timer_flag = 0;
    while (!timer_flag); // Wait for interrupt
}

/*
 * main loop
 */

void main_loop()
{
    // print assignment information
    lcd_printf("Lab03: DAC");
    lcd_locate(0, 1);
    lcd_printf("Group: 06");

    // Initialize peripherals
    dac_initialize();
    timer_initialize();
    led_initialize();

    // Correct voltage values with gain=2x (4.096V full scale)
    uint16_t voltages[3];
    voltages[0] = (uint16_t)((1.0 / 4.096) * 4095);   // 1V
    voltages[1] = (uint16_t)((2.5 / 4.096) * 4095);   // 2.5V
    voltages[2] = (uint16_t)((3.5 / 4.096) * 4095);   // 3.5V

    while (1) {
        dac_output(voltages[0]); // 1V
        delay_ms(500);

        dac_output(voltages[1]); // 2.5V
        delay_ms(2000);

        dac_output(voltages[2]); // 3.5V
        delay_ms(1000);

        TOGGLELED(LED1_PORT);
    }
}