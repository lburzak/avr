#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <avr/io.h>

struct PortConfig {
	volatile uint8_t* PORT;
	volatile uint8_t* PIN;
	volatile uint8_t* DDR;
};

void keypad_set_config(struct PortConfig *config_ptr);
uint8_t keypad_read();
void keypad_set_port();

#endif /* INCFILE1_H_ */