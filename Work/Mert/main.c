#include <xc.h>
#include <p33Fxxxx.h>
#define FCY 12800000UL
#include <libpic30.h>

#include "lcd.h"
#include "led.h"

volatile uint8_t global_counter = 0;

/* Configuration of the Chip */
#pragma config FNOSC = PRIPLL
#pragma config POSCMD = XT
#pragma config FWDTEN = OFF

void timer_init(){
    CLEARBIT(T1CONbits.TON);
    CLEARBIT(T1CONbits.TCS);
    CLEARBIT(T1CONbits.TGATE);
    TMR1 = 0x00;
    T1CONbits.TCKPS = 0b10; 
    PR1 = 50000; 
    IPC0bits.T1IP = 0x01;
    CLEARBIT(IFS0bits.T1IF);
    SETBIT(IEC0bits.T1IE);
    SETBIT(T1CONbits.TON);
}

void __attribute__((__interrupt__)) _T1Interrupt(void){
    global_counter++;
    IFS0bits.T1IF = 0;
}

void display_binary_on_leds(uint8_t value){
    value &= 0x1F; 
    if ((value >> 0) & 0x01) SETLED(PORTAbits.RA4); else CLEARLED(PORTAbits.RA4);
    if ((value >> 1) & 0x01) SETLED(PORTAbits.RA5); else CLEARLED(PORTAbits.RA5);
    if ((value >> 2) & 0x01) SETLED(PORTAbits.RA9); else CLEARLED(PORTAbits.RA9);
    if ((value >> 3) & 0x01) SETLED(PORTAbits.RA10); else CLEARLED(PORTAbits.RA10);
    if ((value >> 4) & 0x01) SETLED(PORTAbits.RA0); else CLEARLED(PORTAbits.RA0);
}

int main(){
    lcd_initialize();
    lcd_clear();

    lcd_printf("Oguzhan Esen\nMert Kulak\nIliasu\n \r");
    __delay_ms(1500);
    lcd_clear();
    led_initialize();
    timer_init();

    while(1){
        lcd_locate(0,0);
        lcd_printf("%d", global_counter);

        display_binary_on_leds(global_counter);

        __delay_ms(500); 
    }

    return 0;
}
