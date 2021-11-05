#define F_CPU 1000000
#include "lcd.h"
#include "keypad.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void) {
	struct PortConfig lcd_config = {
		.DDR = &DDRD,
		.PIN = &PIND,
		.PORT = &PORTD
	};
	
	struct PortConfig keypad_config = {
		.DDR = &DDRA,
		.PIN = &PINA,
		.PORT = &PORTA
	};
	
	lcd_set_config(&lcd_config);
	keypad_set_config(&keypad_config);
	
	DDRA = 0xff;
	
	lcd_init();
	
	lcd_move_cursor(0, 12);
	lcd_write("Lukasz Burzak");
	
	uint8_t value;
	while (1) {
		lcd_home();
		value = keypad_read();
		
		_delay_ms(1);
		
		lcd_number(value);
	}
}

