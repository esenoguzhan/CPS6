#include <xc.h>
#include <p33Fxxxx.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL 
#include <libpic30.h>

#include "lcd.h"
#include "led.h"
#include <stdio.h>

/* Configuration of the Chip */
// Initial Oscillator Source Selection = Primary (XT, HS, EC) Oscillator with PLL
#pragma config FNOSC = PRIPLL
// Primary Oscillator Mode Select = XT Crystal Oscillator mode
#pragma config POSCMD = XT
// Watchdog Timer Enable = Watchdog Timer enabled/disabled by user software
// (LPRC can be disabled by clearing the SWDTEN bit in the RCON register)
#pragma config FWDTEN = OFF

int main(){
// Initialize LCD and LEDs
    lcd_initialize();
    led_init();

    // Clear the screen and set cursor to first row
    lcd_clear();
    lcd_locate(0, 0);
    lcd_printf("Oguzhan Esen\nMert Kulak\nIliasu Salaudin\n \r"); // Sorry if names are wrong :D

    uint8_t counter = 0;
    char buffer[21];

    while(1) {
        // Display counter value on LCD
        lcd_locate(0, 1);
        sprintf(buffer, "Counter: %03d", counter);
        lcd_printf("%s\r", buffer);

        // Show counter in binary on LEDs
        display_counter_on_leds(counter);

        // Wait ~500 ms
        __delay_ms(500);

        // Increment counter (wrap at 32)
        counter = (counter + 1) % 32;
    }

    return 0;
}

