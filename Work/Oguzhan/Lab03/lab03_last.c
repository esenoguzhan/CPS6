#include "lab03.h"

#include <xc.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL
#include <libpic30.h>

#include "types.h"
#include "lcd.h"
#include "led.h"

/*
 * DAC Pin Config
 */
#define DAC_CS_TRIS    TRISDbits.TRISD8
#define DAC_SDI_TRIS   TRISBbits.TRISB10
#define DAC_SCK_TRIS   TRISBbits.TRISB11
#define DAC_LDAC_TRIS  TRISBbits.TRISB13

#define DAC_CS_LAT     LATDbits.LATD8
#define DAC_SDI_LAT    LATBbits.LATB10
#define DAC_SCK_LAT    LATBbits.LATB11
#define DAC_LDAC_LAT   LATBbits.LATB13

#define DAC_SDI_AD1CFG AD1PCFGLbits.PCFG10
#define DAC_SCK_AD1CFG AD1PCFGLbits.PCFG11
#define DAC_LDAC_AD1CFG AD1PCFGLbits.PCFG13

#define DAC_SDI_AD2CFG AD2PCFGLbits.PCFG10
#define DAC_SCK_AD2CFG AD2PCFGLbits.PCFG11
#define DAC_LDAC_AD2CFG AD2PCFGLbits.PCFG13

/*
 * DAC Values
 */
#define DAC_1V      1000
#define DAC_2_5V    2500
#define DAC_3_5V    3500
#define DAC_COMMAND(v12) (0x3000 | ((v12) & 0x0FFF))

/*
 * Global millisecond counter
 */
//volatile uint16_t g_delay_ms = 0;

/*
 * DAC init
 */
void dac_initialize()
{
    // Digital mode
    SETBIT(DAC_SDI_AD1CFG); SETBIT(DAC_SDI_AD2CFG);
    SETBIT(DAC_SCK_AD1CFG); SETBIT(DAC_SCK_AD2CFG);
    SETBIT(DAC_LDAC_AD1CFG); SETBIT(DAC_LDAC_AD2CFG);

    // Output pins
    CLEARBIT(DAC_CS_TRIS);
    CLEARBIT(DAC_SDI_TRIS);
    CLEARBIT(DAC_SCK_TRIS);
    CLEARBIT(DAC_LDAC_TRIS);

    // Default levels
    SETBIT(DAC_CS_LAT);
    SETBIT(DAC_LDAC_LAT);
    CLEARBIT(DAC_SCK_LAT);
    CLEARBIT(DAC_SDI_LAT);
}

/*
 * Bit-bang SPI send
 */
void dac_send(uint16_t word)
{
    CLEARBIT(DAC_CS_LAT);
    int i;
    for (i = 15; i >= 0; --i)
    {
        (word & (1 << i)) ? SETBIT(DAC_SDI_LAT) : CLEARBIT(DAC_SDI_LAT);
        SETBIT(DAC_SCK_LAT); __delay_us(1);
        CLEARBIT(DAC_SCK_LAT); __delay_us(1);
    }
    SETBIT(DAC_CS_LAT);
    CLEARBIT(DAC_SDI_LAT);

    CLEARBIT(DAC_LDAC_LAT); __delay_us(1);
    SETBIT(DAC_LDAC_LAT);
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
    // Enable RTC Oscillator
    __builtin_write_OSCCONL(OSCCONL | 2);

    // Timer1: 500ms using external clock (32.768 kHz), prescaler 1:8
    T1CONbits.TON = 0;
    T1CONbits.TCS = 1;     // External clock
    T1CONbits.TCKPS = 0b01; // Prescaler 1:8
    T1CONbits.TSYNC = 0;
    TMR1 = 0;
    PR1 = 2047;
    IPC0bits.T1IP = 1;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;

    // Timer2: 2000ms using internal clock (12.8 MHz), prescaler 1:256
    T2CONbits.TON = 0;
    T2CONbits.TCS = 0;     // Internal clock
    T2CONbits.TCKPS = 0b11; // Prescaler 1:256
    T2CONbits.TGATE = 0;
    TMR2 = 0;
    PR2 = 99999;
    IPC1bits.T2IP = 3;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CONbits.TON = 1;

    // Timer3: 1000ms using internal clock (12.8 MHz), prescaler 1:256
    T3CONbits.TON = 0;
    T3CONbits.TCS = 0;     // Internal clock
    T3CONbits.TCKPS = 0b11; // Prescaler 1:256
    T3CONbits.TGATE = 0;
    TMR3 = 0;
    PR3 = 49999;
    IPC2bits.T3IP = 2;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
    T3CONbits.TON = 1;
}


// interrupt service routine?



 volatile uint8_t delay_done = 0;
  
 void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{   
    delay_done = 1;
    CLEARBIT(IFS0bits.T1IF);
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    delay_done = 1;
    CLEARBIT(IFS0bits.T2IF);
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{ 
    delay_done = 1;
    CLEARBIT(IFS0bits.T3IF);
}
 void delay_ms()
{
    delay_done = 0;
    while (!delay_done);
}

void main_loop()
{
    // print assignment information
    lcd_printf("Lab03: DAC");
    lcd_locate(0, 1);
    lcd_printf("Group: 06");
    
    while(TRUE)
    {
        // main loop code
        dac_send(DAC_COMMAND(DAC_1V));
        delay_ms();

        dac_send(DAC_COMMAND(DAC_2_5V));
        delay_ms();

        dac_send(DAC_COMMAND(DAC_3_5V));
        delay_ms();


        TOGGLELED(LED1_PORT);
    }
}

 

/*
volatile uint8_t delay_done = 0;

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{   
    delay_done =  1;
    CLEARBIT(IFS0bits.T1IF);
}

 void delay_ms(unsigned int ms)
{
    delay_done = 0;
    unsigned int period = (32768 * ms) / 1000;
    PR1 = period;
    TMR1 = 0;
    while (!delay_done);
}
 
/*
 * main loop
 */

/*
void main_loop()
{
    // print assignment information
    lcd_printf("Lab03: DAC");
    lcd_locate(0, 1);
    lcd_printf("Group: 06");
    
    while(TRUE)
    {
        // main loop code
        dac_send_voltage(1.0);
        delay_ms(500);

        dac_send_voltage(2.5);
        delay_ms(2000);

        dac_send_voltage(3.5);
        delay_ms(1000);

        TOGGLELED(LED1_PORT);
    }
} */