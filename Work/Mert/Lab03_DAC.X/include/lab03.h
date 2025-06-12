#ifndef __LAB03_H__
#define	__LAB03_H__

// initializes the MCP4822 communication for task 1
void dac_initialize();

void dac_send_voltage(float voltage);

void delay_ms(unsigned int ms);

void main_loop();

// initialized the timer for task 2
void timer_initialize();

// contains the loop code from task 1, 2 and 3
void main_loop();

#endif	/* __LAB03_H__ */
