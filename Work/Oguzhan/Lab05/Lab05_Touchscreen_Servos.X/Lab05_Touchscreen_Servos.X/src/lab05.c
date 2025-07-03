#include "lab05.h"
#include <xc.h>
#define FCY 12800000UL
#include <libpic30.h>
#include "lcd.h"

// SERVO CONFIG
#define PWM_CYC_US 20000
#define PWM_PRESCALER 64
#define TMR_PRESCALE_BITS 0b10

void init_servo(char axis) {
    // Use OCx and Tx depending on axis
    // Example for axis 'X' => OC1 and Timer2
}

void set_servo(char axis, uint16_t duty_us) {
    // Compute PWM value based on duty_us
    // Update OCxRS register
}

// TOUCHSCREEN CONFIG

void init_touchscreen() {
    // Configure ADC and digital IOs for touchscreen
}

void set_touchscreen_axis(char axis) {
    // Switch between X or Y axis (configure touchscreen pins accordingly)
    __delay_ms(10); // allow signal to stabilize
}

uint16_t read_touchscreen() {
    // Read from ADC after setting axis
    return 0; // placeholder
}

// CORNER STATE MACHINE

typedef enum {CORNER_1, CORNER_2, CORNER_3, CORNER_4} Corner;

void move_ball_to_corner(Corner corner) {
    // Use set_servo to move ball to corresponding corner
}

void print_position(uint16_t x, uint16_t y) {
    lcd_clear();
    lcd_locate(0, 0);
    lcd_printf("X: %u, Y: %u", x, y);
}

void main_loop() {
    lcd_printf("Lab05: Touchscreen &\\r\\nServos");
    lcd_locate(0, 2);
    lcd_printf("Group: 6");

    init_servo('X');
    init_servo('Y');
    init_touchscreen();

    Corner corner = CORNER_1;

    while (1) {
        move_ball_to_corner(corner);
        __delay_ms(1000);

        set_touchscreen_axis('X');
        uint16_t x = read_touchscreen();

        set_touchscreen_axis('Y');
        uint16_t y = read_touchscreen();

        print_position(x, y);

        __delay_ms(4000); // total 5 seconds including reading & printing

        corner = (corner + 1) % 4;
    }
}
