#include "led.h"

void led_init(void){
	CLEARBIT(LED1_TRIS); // set Pin to Output
	Nop();
	CLEARBIT(LED2_TRIS); // set Pin to Output
	Nop();
	CLEARBIT(LED3_TRIS); // set Pin to Output
	Nop();
	CLEARBIT(LED4_TRIS); // set Pin to Output
	Nop();
	CLEARBIT(LED5_TRIS); // set Pin to Output
	Nop();
}

// Function to display the counter value on LEDs (5 bits)
void display_counter_on_leds(uint8_t count) {
    if (count & (1 << 0)) SETLED(LED1_PORT); else CLEARLED(LED1_PORT);
    if (count & (1 << 1)) SETLED(LED2_PORT); else CLEARLED(LED2_PORT);
    if (count & (1 << 2)) SETLED(LED3_PORT); else CLEARLED(LED3_PORT);
    if (count & (1 << 3)) SETLED(LED4_PORT); else CLEARLED(LED4_PORT);
    if (count & (1 << 4)) SETLED(LED5_PORT); else CLEARLED(LED5_PORT);
}
