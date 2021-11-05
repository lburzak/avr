#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

#include <stdint.h>

struct PortConfig {
	volatile uint8_t* PORT;
	volatile uint8_t* PIN;
	volatile uint8_t* DDR;
};


#endif /* PORT_CONFIG_H_ */