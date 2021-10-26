#define F_CPU 1000000
#include "keypad.h"
#include <util/delay.h>

int main(void)
{
	// Ustawia pierwsza tetrade portu klawiatury na wejscie
	DDRA = 0xf0;
	
	// Ustawia port ledow na wejscie
	DDRB = 0xff;
	
	while (1) {
		// Ustawia port ledow na wartosc odpowiadajaca wcisnietemu przyciskowi
		PORTB = keypad_read();
		_delay_ms(1);
	}
}

