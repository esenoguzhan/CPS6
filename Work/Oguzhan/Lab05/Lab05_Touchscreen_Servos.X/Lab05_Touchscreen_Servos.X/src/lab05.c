#include "lab05.h"
#include <xc.h>
#define FCY 12800000UL
#include <libpic30.h>
#include "lcd.h"

#define PWM_CYCLE_US 20000
#define TCKPS_64 0b10
#define PRESCALER 64

// Timer period for 20ms PWM cycle
#define PR2_VALUE ((FCY / PRESCALER) * PWM_CYCLE_US / 1000000)

void init_servo(char axis) {
    if (axis == 'X') {
        CLEARBIT(TRISDbits.TRISD7); // OC8 as output

        // Timer2 setup
        CLEARBIT(T2CONbits.TON);
        CLEARBIT(T2CONbits.TCS);
        CLEARBIT(T2CONbits.TGATE);
        TMR2 = 0x00;
        T2CONbits.TCKPS = TCKPS_64;
        PR2 = PR2_VALUE;
        CLEARBIT(IFS0bits.T2IF);
        CLEARBIT(IEC0bits.T2IE);

        // OC8 setup
        OC8CON = 0x0000;
        OC8R = 1500 * (FCY / PRESCALER) / 1000000;
        OC8RS = OC8R;
        OC8CON = 0x0006; // PWM mode, use Timer2

        SETBIT(T2CONbits.TON); // start Timer2
    }
    // Add Y-axis (e.g., OC7/Timer3) if available
}

void set_servo(char axis, uint16_t duty_us) {
    uint16_t compare_val = (duty_us * (FCY / PRESCALER)) / 1000000;
    if (axis == 'X') {
        OC8RS = compare_val;
    }
}

void init_touchscreen() {
    CLEARBIT(AD1CON1bits.ADON);

    SETBIT(TRISEbits.TRISE1);
    SETBIT(TRISEbits.TRISE2);
    SETBIT(TRISEbits.TRISE3);

    CLEARBIT(AD1PCFGHbits.PCFG15); // AN15
    CLEARBIT(AD1PCFGHbits.PCFG9);  // AN9

    AD1CON1bits.AD12B = 0;
    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 0x7;

    AD1CON2 = 0;
    CLEARBIT(AD1CON3bits.ADRC);
    AD1CON3bits.SAMC = 0x1F;
    AD1CON3bits.ADCS = 0x2;

    SETBIT(AD1CON1bits.ADON);
}

void set_touchscreen_axis(char axis) {
    if (axis == 'X') {
        CLEARBIT(TRISEbits.TRISE1);
        CLEARBIT(TRISEbits.TRISE2);
        CLEARBIT(TRISEbits.TRISE3);
        CLEARBIT(PORTEbits.RE1);
        SETBIT(PORTEbits.RE2);
        SETBIT(PORTEbits.RE3);
    } else if (axis == 'Y') {
        CLEARBIT(TRISEbits.TRISE1);
        CLEARBIT(TRISEbits.TRISE2);
        CLEARBIT(TRISEbits.TRISE3);
        SETBIT(PORTEbits.RE1);
        CLEARBIT(PORTEbits.RE2);
        CLEARBIT(PORTEbits.RE3);
    }
    __delay_ms(10);
}

uint16_t read_touchscreen() {
    if (!AD1CON1bits.ADON) return 0;

    if (PORTEbits.RE1 == 0) {
        AD1CHS0bits.CH0SA = 0x0F; // AN15 for X
    } else {
        AD1CHS0bits.CH0SA = 0x09; // AN9 for Y
    }

    SETBIT(AD1CON1bits.SAMP);
    while (!AD1CON1bits.DONE);
    CLEARBIT(AD1CON1bits.DONE);
    return ADC1BUF0;
}

typedef enum {CORNER_1, CORNER_2, CORNER_3, CORNER_4} Corner;

void move_ball_to_corner(Corner corner) {
    switch (corner) {
        case CORNER_1: set_servo('X', 2000); break;
        case CORNER_2: set_servo('X', 1000); break;
        case CORNER_3: set_servo('X', 1000); break;
        case CORNER_4: set_servo('X', 2000); break;
    }
}

void print_position(uint16_t x, uint16_t y) {
    lcd_clear();
    lcd_locate(0, 0);
    lcd_printf("X: %u, Y: %u\\r", x, y);
}

void main_loop() {
    lcd_printf("Lab05: Touchscreen &\\r\\nServos");
    lcd_locate(0, 2);
    lcd_printf("Group: YourName");

    init_servo('X');
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

        __delay_ms(4000);
        corner = (corner + 1) % 4;
    }
}
