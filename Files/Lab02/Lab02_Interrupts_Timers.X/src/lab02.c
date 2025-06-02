#include "lab02.h"

#include <xc.h>
#include <p33Fxxxx.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL
#include <libpic30.h>

#include "types.h"
#include "lcd.h"
#include "led.h"

#define FCY_EXT 32768

// Timer counters m: minutes, s: seconds, ms: milliseconds
volatile uint16_t mTime = 0;
volatile uint16_t sTime = 0;
volatile uint16_t msTime = 0;


void initialize_timer()
{
    // Enable RTC Oscillator -> this effectively does OSCCONbits.LPOSCEN = 1
    // but the OSCCON register is lock protected. That means you would have to 
    // write a specific sequence of numbers to the register OSCCONL. After that 
    // the write access to OSCCONL will be enabled for one instruction cycle.
    // The function __builtin_write_OSCCONL(val) does the unlocking sequence and
    // afterwards writes the value val to that register. (OSCCONL represents the
    // lower 8 bits of the register OSCCON)
    __builtin_write_OSCCONL(OSCCONL | 2);
    
    // Timer2 config
    T2CONbits.TON = 0; // Disable Timer
    T2CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    CLEARBIT(T2CONbits.TCS); // Select the internal instruction 12.8MHz cycle clock
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    TMR2 = 0x00; // Clear timer register
    PR2 = FCY / 256 / 500; // Load the period value
    IPC1bits.T2IP = 0x03; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS1bits.T2IF); // Clear Timer2 Interrupt Flag
    SETBIT(IEC1bits.T2IE); // Enable Timer2 interrupt
    SETBIT(T2CONbits.TON); // Start Timer
    
    // Timer1 config
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    T1CONbits.TCS = 1; // Select the external 32.768kHz clock source
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TSYNC = 0; // Disable Synchronization
    TMR1 = 0x00; // Clear timer register
    PR1 = FCY_EXT / 256; // Load the period value 
    IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS0bits.T1IF); // Clear Timer1 Interrupt Flag
    SETBIT(IEC0bits.T1IE); // Enable Timer1 interrupt
    SETBIT(T1CONbits.TON); // Start Timer
    
    // Timer3 config
    T3CONbits.TON = 0; // Disable Timer
    T3CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    CLEARBIT(T3CONbits.TCS); // Select the internal instruction 12.8MHz cycle clock
    T3CONbits.TGATE = 0; // Disable Gated Timer mode
    TMR3 = 0x00; // Clear timer register
    PR3 = 0xFFFF; // highest value possible
    CLEARBIT(IEC2bits.T3IE); // Enable Timer3 interrupt
    SETBIT(T3CONbits.TON); // Start Timer
}

void timer_loop()
{
    // print assignment information
    lcd_printf("Lab02: Int & Timer");
    lcd_locate(0, 1);
    lcd_printf("Group: 6");
    
    uint16_t loopCount = 0;
    uint16_t startTMR3, endTMR3, diff;
    double msElapsed;
    

    while (TRUE)
    { 
        if (loopCount == 0)
        {
            TMR3 = 0; // Reset Timer3 at start
            startTMR3 = 0;
        }

        loopCount++;

        if (loopCount >= 2000)
        {
            TOGGLELED(LED3_PORT); // Toggle LED3

            // Calculate loop execution time
            endTMR3 = TMR3;
            diff = endTMR3 - startTMR3;
            msElapsed = ((double)diff / (double)FCY) * 1000.0;

            // Format uptime: mm:ss.xxx
            // char uptimeStr[20];
            // snprintf(uptimeStr, sizeof(uptimeStr), "%02u:%02u.%03u", mTime, sTime, msTime);
            
            // Display uptime
            // lcd_locate(0, 5);
            // lcd_printf("Uptime: %s", uptimeStr);
            
            // Display uptime
            lcd_locate(0, 5);
            lcd_printf("%02u:%02u.%03u", mTime, sTime, msTime);

            // Display TMR3 counts
            lcd_locate(0, 6);
            lcd_printf("T3: %u cycles", diff);

            // Display TMR3 time in ms
            lcd_locate(0, 7);
            lcd_printf("Time: %.4f ms", msElapsed);

            loopCount = 0; // Reset loop count
        }
    }
}

void __attribute__((__interrupt__, __shadow__, __auto_psv__)) _T1Interrupt(void)
{ // invoked every second
    TOGGLELED(LED2_PORT); // Toggle LED2
    sTime++; // sum up s timer
    
    // roll up seconds to minutes
    if(sTime >= 60){
        mTime++;
        sTime = 0;
    }
    
    CLEARBIT(IFS0bits.T1IF); // Clear the interrupt flag
}

void __attribute__((__interrupt__, __shadow__, __auto_psv__)) _T2Interrupt(void)
{ // invoked every 2 millisecond
    TOGGLELED(LED1_PORT); // Toggle LED1
    msTime += 2; // sum up ms timer
    
    //roll up ms to s
    if(msTime >= 1000){
        msTime = 0;
    }
    
    CLEARBIT(IFS1bits.T2IF); // Clear the interrupt flag
}
