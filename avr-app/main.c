#define F_CPU 1000000
#include "lcd.h"
#include <avr/io.h>

int main(void) {
	struct PortConfig lcd_config = {
		.DDR = &DDRD,
		.PIN = &PIND,
		.PORT = &PORTD
	};
	
	lcd_set_config(&lcd_config);
	
	lcd_init();
	lcd_clear();
	lcd_move_cursor(1, 3);
	lcd_write("hello");
}

