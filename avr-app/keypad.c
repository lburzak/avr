#include <avr/io.h>

uint8_t keypad_read() {
	uint8_t col_state;
	uint8_t keycode = 0;
	
	// Iteruje po indeksach kolumn
	for (uint8_t col = 0; col <= 3; col++) {
		
		// Ustawia 0 w bicie odpowiadajacym danej kolumnie
		PORTA = ~(1 << (col + 4));

		// Odczytuje stan danej kolumny
		col_state = PINA & 0x0f;	
		
		// Jezeli ktorys przycisk w kolumnie jest wcisniety
		if (col_state < 0x0f) {
			if (keycode > 0)
				return 0xff;
			
			switch (col_state) {
				case 0b00001110: keycode = col + 1; break;
				case 0b00001101: keycode = col + 5; break;
				case 0b00001011: keycode = col + 9; break;
				case 0b00000111: keycode = col + 13; break;
				default: return 0xff;
			}
		}
	}
	
	return keycode;
}