#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define LCD_DDR DDRD
#define LCD_PORT PORTD
#define RS 1
#define E 0
#define LCD_DB4 4
#define LCD_DB5 5
#define LCD_DB6 6
#define LCD_DB7 7

#define CMD_CLEAR				1 << 0
#define CMD_ENTRY_MODE_SET		1 << 2
#define CMD_DISPLAY_CONTROL		1 << 3
#define CMD_FUNCTION_SET		1 << 5
#define CMD_SET_DDRAM			1 << 7

#define FLAG_FORWARD_SHIFT		1 << 1
#define FLAG_DISPLAY_SHIFT		1 << 0

#define FLAG_8_BIT_MODE			1 << 4
#define FLAG_TWO_ROWS			1 << 3
#define FLAG_WIDE_CHARACTERS	1 << 2

#define FLAG_ENABLE_DISPLAY		1 << 2
#define FLAG_SHOW_CURSOR		1 << 1
#define FLAG_BLINK_CURSOR		1 << 0

void lcd_send_nibble(uint8_t byte) {
	// Aktywuje przesyl danych
	LCD_PORT |= 1 << E;

	// Ustawia dane w porcie
	LCD_PORT = byte | (LCD_PORT & 0x0F);

	// Zatwierdza przesyl danych
	LCD_PORT &= ~(1 << E);
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
	LCD_PORT &= ~(1 << RS);

	// Przesyla wskazany bajt komendy
	lcd_send(command);

	// Przelacza LCD w tryb danych
	LCD_PORT |= 1 << RS;

	_delay_ms(5);
}

void lcd_init() {
	// Ustawia wszystkie linie na wyjscie
	LCD_DDR = (0xF0) | (1 << RS) | (1 << E);
	LCD_PORT = 0;

	// Ustawia tryb 4-bitowy
	lcd_cmd(0x02);

	// Ustawia parametry wyswietlania
	lcd_cmd(CMD_FUNCTION_SET | FLAG_TWO_ROWS);

	// Ustawia tryb pracy wyswietlacza
	lcd_cmd(CMD_ENTRY_MODE_SET | FLAG_FORWARD_SHIFT);

	// Ustawia stan wyswietlacza
	lcd_cmd(CMD_DISPLAY_CONTROL | FLAG_ENABLE_DISPLAY);
	
	// Czysci LCD
	lcd_cmd(CMD_CLEAR);
}

void lcd_move_cursor(uint8_t x, uint8_t y) {
	// Przenosi kursor w wyznaczone miejsce
	lcd_cmd(CMD_SET_DDRAM | (x << 6) | y );
}

void lcd_write(char *chars) {
	// Iteruje po znakach we wskazanym lancuchu
	for (uint8_t i = 0; chars[i]; i++) {
		// Jezeli brakuje miejsca w linii, przechodzi do nastepnej
		if (i==16)
			lcd_move_cursor(1,0);

		// Wypisuje znak
		lcd_send(chars[i]);
	}
}

void lcd_clear() {
	lcd_cmd(CMD_CLEAR);
}