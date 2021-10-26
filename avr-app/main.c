#define F_CPU 1000000
#include "keypad.h"
#include <util/delay.h>

int main(void)
{	
	// Ustawia port ledow na wejscie
	DDRB = 0xff;
	PORTA = 0xf0;
	
	// Tworzy strukture przechowujaca konfiguracje portu klawiatury
	struct PortConfig config = {
		.PORT = &PORTA,
		.PIN = &PINA,
		.DDR = &DDRA
	};
	
	// Konfiguruje klawiature
	keypad_set_config(&config);
	
	while (1) {
		// Ustawia port ledow na wartosc odpowiadajaca wcisnietemu przyciskowi
		PORTB = keypad_read();
		_delay_ms(1);
	}
}

