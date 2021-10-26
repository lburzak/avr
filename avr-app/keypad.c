#include "keypad.h"
#include <avr/io.h>

#define KEYCODE_ERROR 0xff

static struct PortConfig *config_ptr;

void keypad_set_config(struct PortConfig *new_config_ptr) {
	// Zmienia wskaznik konfiguracji na okreslony w parametrze
	config_ptr = new_config_ptr;
	
	// Ustawia pierwsza tetrade portu klawiatury na wejscie
	*config_ptr->DDR = 0xf0;
}

uint8_t parse_keycode(uint8_t col_state) {
	// Zapisuje kod przycisku do zmiennej
	switch (col_state) {
		case 0b00001110: keycode = col + 1; break;
		case 0b00001101: keycode = col + 5; break;
		case 0b00001011: keycode = col + 9; break;
		case 0b00000111: keycode = col + 13; break;
		default:
			// Zostalo wcisniete wiecej przyciskow w kolumnie, zwraca kod bledu
			return KEYCODE_ERROR;
	}
}

uint8_t keypad_read() {
	uint8_t col_state;
	uint8_t keycode = 0;
	
	// Iteruje po indeksach kolumn
	for (uint8_t col = 0; col <= 3; col++) {
		
		// Ustawia 0 w bicie odpowiadajacym danej kolumnie
		*config_ptr->PORT = ~(1 << (col + 4));

		// Odczytuje stan danej kolumny
		col_state = *config_ptr->PIN & 0x0f;
		
		// Jezeli ktorys przycisk w kolumnie jest wcisniety
		if (col_state < 0x0f) {
			// Sprawdza czy podczas iteracji po kolumnach zostal juz wykryty wcisniety przycisk
			if (keycode > 0)
				// Zwraca kod bledu
				return KEYCODE_ERROR;
			
			keycode = parse_keycode(col_state);
		}
	}
	
	return keycode;
}