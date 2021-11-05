#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "port_config.h"

#define RS 1
#define E 0

#define CMD_CLEAR				1 << 0
#define CMD_HOME				1 << 1
#define CMD_ENTRY_MODE_SET		1 << 2
#define CMD_DISPLAY_CONTROL		1 << 3
#define CMD_FUNCTION_SET		1 << 5
#define CMD_SET_DDRAM			1 << 7

enum EntryMode {
	FORWARD_SHIFT = 1 << 1,
	DISPLAY_SHIFT = 1 << 0
};

enum FunctionSet {
	EIGHT_BIT_MODE = 1 << 4,
	TWO_ROWS = 1 << 3,
	WIDE_CHARACTERS = 1 << 2	
};

enum DisplayControl {
	ENABLE_DISPLAY = 1 << 2,
	SHOW_CURSOR = 1 << 1,
	BLINK_CURSOR = 1 << 0
};

static struct PortConfig *config_ptr;

void lcd_set_config(struct PortConfig *config);
void lcd_send_nibble(uint8_t byte);
void lcd_send(uint8_t byte);
void lcd_cmd(uint8_t command);

void lcd_init() {
	// Ustawia wszystkie linie na wyjscie
	*config_ptr->DDR = (0xF0) | (1 << RS) | (1 << E);
	*config_ptr->PORT = 0;

	// Ustawia tryb 4-bitowy
	lcd_cmd(0x02);

	// Ustawia parametry wyswietlania
	lcd_cmd(CMD_FUNCTION_SET | TWO_ROWS);

	// Ustawia tryb pracy wyswietlacza
	lcd_cmd(CMD_ENTRY_MODE_SET | FORWARD_SHIFT);

	// Ustawia stan wyswietlacza
	lcd_cmd(CMD_DISPLAY_CONTROL | ENABLE_DISPLAY);
	
	// Czysci LCD
	lcd_cmd(CMD_CLEAR);
}

static volatile uint8_t cursor_y = 0;

void lcd_move_cursor(uint8_t x, uint8_t y) {
	// Przenosi kursor w wyznaczone miejsce
	lcd_cmd(CMD_SET_DDRAM | (x << 6) | y );
	cursor_y = y;
}

void lcd_write(char *chars) {
	// Iteruje po znakach we wskazanym lancuchu
	for (uint8_t i = 0; chars[i]; i++, cursor_y++) {
		// Jezeli brakuje miejsca w linii, przechodzi do nastepnej
		if (cursor_y == 16)
			lcd_move_cursor(1,0);

		// Wypisuje znak
		lcd_send(chars[i]);
	}
}

void lcd_clear() {
	lcd_cmd(CMD_CLEAR);
}

void lcd_send_nibble(uint8_t byte) {
	// Aktywuje przesyl danych
	*config_ptr->PORT |= 1 << E;

	// Ustawia dane w porcie
	*config_ptr->PORT = byte | (*config_ptr->PORT & 0x0F);

	// Zatwierdza przesyl danych
	*config_ptr->PORT &= ~(1 << E);
}

/** Przesyla bajt */
void lcd_send(uint8_t byte) {
	// Przesyla starszy polbajt
	lcd_send_nibble(byte & 0xF0);

	// Odczekuje jeden cykl
	asm volatile("nop");

	// Przesyla mlodszy polbajt
	lcd_send_nibble((byte & 0x0F) << 4);

	_delay_us(50);
}

void lcd_cmd(uint8_t command) {
	// Przelacza LCD w tryb komend
	*config_ptr->PORT &= ~(1 << RS);

	// Przesyla wskazany bajt komendy
	lcd_send(command);

	// Przelacza LCD w tryb danych
	*config_ptr->PORT |= 1 << RS;

	_delay_ms(5);
}

void lcd_set_config(struct PortConfig *new_config_ptr) {
	// Zmienia wskaznik konfiguracji na okreslony w parametrze
	config_ptr = new_config_ptr;
	
	// Ustawia pierwsza tetrade portu klawiatury na wejscie
	*config_ptr->DDR = 0xf0;
}

/** Wypisuje liczbe z zakresu 0-19 */
void lcd_number(uint8_t num) {
	if (num > 19)
		return lcd_write("xx");
	
	if (num > 9) {
		lcd_send(num / 10 + '0');
		lcd_send(num % 10 + '0');
	} else {
		lcd_send(num + '0');
		lcd_send(' ');
	}
}

void lcd_home() {
	lcd_cmd(CMD_HOME);
}