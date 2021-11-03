#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "port_config.h"
#include <avr/io.h>

void keypad_set_config(struct PortConfig *config_ptr);
uint8_t keypad_read();

#endif /* INCFILE1_H_ */