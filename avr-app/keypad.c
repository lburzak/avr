#include "keypad.h"
#include <avr/io.h>

#define KEYPAD_ROWS 4
#define KEYCODE_ERROR 0xff

static struct PortConfig *config_ptr;

uint8_t read_column(uint8_t index);
uint8_t get_row_index(uint8_t column_value);

uint8_t keypad_read() {
	uint8_t column_value;
	uint8_t row_index;
	uint8_t keycode = 0;
	
	// Iteruje po indeksach kolumn
	for (uint8_t column_index = 0; column_index <= 3; column_index++) {
		// Odczytuje wartosc obecnej kolumny
		column_value = read_column(column_index);

		// Przechodzi do nastepnej kolumny, jezeli zaden przycisk w kolumnie nie jest wcisniety
		if (column_value >= 0x0f)
			continue;
			
		// Zwraca kod bledu, jezeli w jednej z poprzednich kolumn zostal wykryty wcisniety przycisk
		if (keycode > 0)
			return KEYCODE_ERROR;
		
		// Oblicza indeks wiersza w ktorym jest wcisniety przycisk
		row_index = get_row_index(column_value);
		
		// Zwraca kod bledu, jezeli w obecnej kolumnie zostalo wcisnietych wiele przyciskow
		if (row_index >= KEYPAD_ROWS)
			return KEYCODE_ERROR;
		
		// Zapisuje kod odczytanego przycisku
		keycode = row_index * 4 + column_index + 1;
	}
	
	return keycode;
}

/*
Zmienia konfiguracje portu.
*/
void keypad_set_config(struct PortConfig *new_config_ptr) {
	// Zmienia wskaznik konfiguracji na okreslony w parametrze
	config_ptr = new_config_ptr;
	
	// Ustawia pierwsza tetrade portu klawiatury na wejscie
	*config_ptr->DDR = 0xf0;
}

/*
Zwraca wartosc kolumny o podanym indeksie.
*/
uint8_t read_column(uint8_t index) {
	// Ustawia klawiature w tryb odczytu danej kolumny
	*config_ptr->PORT = ~(1 << (index + 4));
	
	// Zwraca wartosc danej kolumny
	return *config_ptr->PIN & 0x0f;
}

/*
Zwraca indeks wiersza w ktorym jest wcisniety przycisk.
Jezeli zostalo wcisniete wiele przyciskow, zwraca -1.
*/
uint8_t get_row_index(uint8_t column_value) {
	switch (column_value) {
		case 0b00001110: return 0;
		case 0b00001101: return 1;
		case 0b00001011: return 2;
		case 0b00000111: return 3;
		default:
			return -1;
	}
}
