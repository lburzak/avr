#include "keypad.h"
#include <avr/io.h>

#define KEYPAD_ROWS 4
#define KEYCODE_ERROR 0xff

static struct PortConfig *config_ptr;

uint8_t read_column(uint8_t index);
uint8_t get_row_index(uint8_t column_value);
uint8_t is_not_power_of_2(uint8_t x);

uint8_t keypad_read() {
	uint8_t column_value;
	uint8_t row_index;
	uint8_t keycode = 0;
	
	// Iteruje po indeksach kolumn
	for (uint8_t column_index = 0; column_index <= 3; column_index++) {
		// Odczytuje wartosc obecnej kolumny
		column_value = read_column(column_index);

		// Przechodzi do nastepnej kolumny, jezeli zaden przycisk w kolumnie nie jest wcisniety
		if (!column_value)
			continue;
			
		// Zwraca kod bledu, jezeli w jednej z poprzednich kolumn zostal wykryty wcisniety przycisk,
		// lub wiele przyciskow jest wcisnietych w obecnej kolumnie
		if (keycode > 0 || is_not_power_of_2(column_value))
			return KEYCODE_ERROR;
		
		// Zapisuje kod odczytanego przycisku
		keycode = get_row_index(column_value) * KEYPAD_ROWS + column_index + 1;
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
	config_ptr->DDR = 0xf0;
}

/*
Zwraca wartosc kolumny o podanym indeksie.
*/
uint8_t read_column(uint8_t index) {
	// Ustawia klawiature w tryb odczytu danej kolumny
	*config_ptr->PORT = ~(1 << (index + 4));
	
	// Zwraca wartosc danej kolumny
	return ~*config_ptr->PIN & 0x0f;
}

/*
Zwraca indeks wiersza w ktorym jest wcisniety przycisk.
*/
uint8_t get_row_index(uint8_t column_value) {
	uint8_t i = 0;
	
	while (column_value > 1) {
		column_value >>= 1;
		i++;
	}
		
	return i;
}

/*
Sprawdza czy wartosc nie jest potega liczby 2
*/
uint8_t is_not_power_of_2(uint8_t x) {
	return (x != 1) && (x & (x - 1));
}
