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

    else if (axis == 'Y') {
        CLEARBIT(TRISDbits.TRISD6); // OC7 as output

        // Timer3 setup
        CLEARBIT(T3CONbits.TON);
        CLEARBIT(T3CONbits.TCS);
        CLEARBIT(T3CONbits.TGATE);
        TMR3 = 0x00;
        T3CONbits.TCKPS = TCKPS_64;
        PR3 = PR2_VALUE;  // same 20ms PWM period
        CLEARBIT(IFS0bits.T3IF);
        CLEARBIT(IEC0bits.T3IE);

        // OC7 setup
        OC7CON = 0x0000;
        OC7R = 1500 * (FCY / PRESCALER) / 1000000;
        OC7RS = OC7R;
        OC7CON = 0x0006; // PWM mode, use Timer3

        SETBIT(T3CONbits.TON); // start Timer3
    }
    set_servo('X',1500);
    set_servo('Y',1500);
    __delay_ms(1000);
}


void set_servo(char axis, uint16_t duty_us) {
    uint16_t compare_val = (-1*(duty_us-20000) * (FCY / PRESCALER)) / 1000000;
    if (axis == 'X') {
        OC8RS = compare_val;
    } else if (axis == 'Y') {
        OC7RS = compare_val;  // Y-axis servo on OC7
    }
}

void init_touchscreen() {
    CLEARBIT(AD1CON1bits.ADON);
    
    SETBIT(TRISBbits.TRISB15);
    SETBIT(TRISBbits.TRISB9);
    
    SETBIT(TRISEbits.TRISE1);
    SETBIT(TRISEbits.TRISE2);
    SETBIT(TRISEbits.TRISE3);
    

    AD1PCFGHbits.PCFG20 = 1; // RE8 as digital (for buttons)
    AD1PCFGLbits.PCFG15 = 0; // AN15 as analog input (X)
    AD1PCFGLbits.PCFG9  = 0; // AN9 as analog input (Y)

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
        Nop();
        CLEARBIT(TRISEbits.TRISE2);
        Nop();

        CLEARBIT(TRISEbits.TRISE3);
        Nop();

        CLEARBIT(PORTEbits.RE1);
        Nop();

        SETBIT(PORTEbits.RE2);
        Nop();
        SETBIT(PORTEbits.RE3);
        Nop();
    } else if (axis == 'Y') {
        CLEARBIT(TRISEbits.TRISE1);
        Nop();
        CLEARBIT(TRISEbits.TRISE2);
        Nop();
        CLEARBIT(TRISEbits.TRISE3);
        Nop();
        SETBIT(PORTEbits.RE1);
        Nop();
        CLEARBIT(PORTEbits.RE2);
        Nop();
        CLEARBIT(PORTEbits.RE3);
        Nop();
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








//typedef enum {CORNER_1, CORNER_2, CORNER_3, CORNER_4} Corner;

void move_ball_to_corner(int corner) {
    switch (corner) {
        case 3:
            set_servo('X', 2000);
            set_servo('Y', 2000);
            break;
        case 2:
            set_servo('X', 1000);
            set_servo('Y', 2000);
            break;
        case 1:
            set_servo('X', 1000);
            set_servo('Y', 1000);
            break;
        case 0:
            set_servo('X', 2000);
            set_servo('Y', 1000);
            break;
    }
}


void print_position(uint16_t x, uint16_t y) {
    lcd_clear();
    lcd_locate(0, 0);
    lcd_printf("X: %u, Y: %u", x, y);
    
}



void main_loop() {
    lcd_printf("Lab05: Touchscreen &\\r\\nServos");
    lcd_locate(0, 2);
    lcd_printf("Group: 06");

    
    init_servo('X');
    init_servo('Y');
    init_touchscreen();

    int corner = 0;
    
    
    
    while (1) {
        
                
        set_touchscreen_axis('X');
        uint16_t x = read_touchscreen();

        set_touchscreen_axis('Y');
        uint16_t y = read_touchscreen();

        print_position(x, y);
        
        move_ball_to_corner(corner);
        //set_servo("X", 3820);


       

        __delay_ms(3000);
        corner = (corner + 1) % 4;
        lcd_locate(0, 4);
        lcd_printf("corner: %d", corner);
    }
}
