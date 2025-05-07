#include <xc.h>
#include <p33Fxxxx.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL 
#include <libpic30.h>

#include "lcd.h"
#include "led.h"

/* Configuration of the Chip */
// Initial Oscillator Source Selection = Primary (XT, HS, EC) Oscillator with PLL
#pragma config FNOSC = PRIPLL
// Primary Oscillator Mode Select = XT Crystal Oscillator mode
#pragma config POSCMD = XT
// Watchdog Timer Enable = Watchdog Timer enabled/disabled by user software
// (LPRC can be disabled by clearing the SWDTEN bit in the RCON register)
#pragma config FWDTEN = OFF

// Bit Value macro
#define BV(i) (1 << (i))  

// declare counter
volatile uint8_t counter = 0;
void display_counter_on_leds(uint8_t);
void display_names_on_lcd();

int main(){
    // initailize lcd and leds
    lcd_initialize();
    led_init();
    
    // display names on lcd in alphabetical order
    display_names_on_lcd();
    __delay_ms(1500);
    lcd_clear();
    
    // infinite loop
    while(1){
        counter++;
        lcd_locate(0, 0);
        lcd_printf("Counter: %d", counter);
        display_counter_on_leds(counter);
        __delay_ms(500);
    }
}

void display_counter_on_leds(uint8_t count){
    count &= 0x1f;  // &= BV(0) | BV(1) | BV(2) | BV(3) | BV(4);
    
    if(count & BV(0)) {SETLED(LED1_PORT);} else {CLEARLED(LED1_PORT);}
    if(count & BV(1)) {SETLED(LED2_PORT);} else {CLEARLED(LED2_PORT);}
    if(count & BV(2)) {SETLED(LED3_PORT);} else {CLEARLED(LED3_PORT);}
    if(count & BV(3)) {SETLED(LED4_PORT);} else {CLEARLED(LED4_PORT);}
    if(count & BV(4)) {SETLED(LED5_PORT);} else {CLEARLED(LED5_PORT);}
}

void display_names_on_lcd(){
    lcd_clear();
    lcd_locate(0, 0);
    
    lcd_printf("Iliasu Salaudeen");
    lcd_locate(0, 1);
            
    lcd_printf("Mert Kulak");
    lcd_locate(0, 2);
    
    lcd_printf("Oguzhan Esen");
}

