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

/* #define DAC_CS_TRIS TRISDbits.TRISD8
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
#define DAC_LDAC_AD2CFG AD2PCFGLbits.PCFG13 */


/*
 * Bit Macros
 
#define SETBIT(x)      ((x) = 1)
#define CLEARBIT(x)    ((x) = 0)
#define TOGGLEBIT(x)   ((x) ^= 1)
 * */

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
volatile uint16_t g_delay_ms = 0;

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
    for (int i = 15; i >= 0; --i)
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
 * Timer (RTC) Setup
 */
#define FCY_EXT   32768UL

#define TCKPS_1   0x00
#define TCKPS_8   0x01
#define TCKPS_64  0x02
#define TCKPS_256 0x03

void timer_initialize()
{
    // Enable 32.768 kHz oscillator (SOSC)
    __builtin_write_OSCCONL(OSCCONL | 0x02);

    // Wait a bit for oscillator to stabilize
    for (volatile int i = 0; i < 10000; ++i) Nop();

    // Timer1 config
    T1CONbits.TON = 0;
    T1CONbits.TCS = 1;               // External clock (SOSC)
    T1CONbits.TCKPS = TCKPS_1;       // 1:1 prescaler

    TMR1 = 0;
    PR1 = 33 - 1;                    // ~1 ms at 32.768 kHz
    IFS0bits.T1IF = 0;              // Clear interrupt flag
    IEC0bits.T1IE = 1;              // Enable interrupt
    T1CONbits.TON = 1;              // Start Timer1
}

/*
 * Timer1 ISR
 */
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    if (g_delay_ms > 0)
        --g_delay_ms;
}

/*
 * Delay using RTC interrupt counter
 */
void wait_ms(uint16_t ms)
{
    g_delay_ms = ms;
    while (g_delay_ms > 0);  // wait for counter to expire
}

/*
 * Main Loop
 */
void main_loop()
{
    lcd_printf("Lab03: DAC");
    lcd_locate(0, 1);
    lcd_printf("Group: 6");

    dac_initialize();
    timer_initialize();
    led_initialize();

    while (TRUE)
    {
        dac_send(DAC_COMMAND(DAC_1V));
        wait_ms(500);

        dac_send(DAC_COMMAND(DAC_2_5V));
        wait_ms(2000);

        dac_send(DAC_COMMAND(DAC_3_5V));
        wait_ms(1000);

        led_toggle(1);
    }
}