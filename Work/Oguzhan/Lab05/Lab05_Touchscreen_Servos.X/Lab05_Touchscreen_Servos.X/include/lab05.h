#ifndef LAB05_H
#define LAB05_H

#include <stdint.h>

void main_loop();
void init_servo(char axis);
void set_servo(char axis, uint16_t duty_us);
void init_touchscreen();
void set_touchscreen_axis(char axis);
uint16_t read_touchscreen();

#endif /* LAB05_H */
