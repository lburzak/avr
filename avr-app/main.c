#define F_CPU 1000000
#include "lcd.h"
#include "keypad.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void) {
	// Konfiguruje LCD na porcie D
	struct PortConfig lcd_config = {
		.DDR = &DDRD,
		.PIN = &PIND,
		.PORT = &PORTD
	};
	
	lcd_set_config(&lcd_config);
	
	// Konfiguruje klawiature na porcie A
	struct PortConfig keypad_config = {
		.DDR = &DDRA,
		.PIN = &PINA,
		.PORT = &PORTA
	};
	
	keypad_set_config(&keypad_config);
	
	DDRA = 0xff;
	
	// Inicializuje klawiature
	lcd_init();
	
	// Wypisuje imie i nazwisko na pozycji x=0, y=12
	lcd_move_cursor(0, 12);
	lcd_write("Lukasz Burzak");
	
	// Wypisuje wartosci odpowiadajace naciskanym przyciskom
	uint8_t value;
	while (1) {
		lcd_home();
		value = keypad_read();
		
		_delay_ms(1);
		
		lcd_number(value);
	}
}

