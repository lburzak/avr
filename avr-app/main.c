#include <avr/io.h>
#define F_CPU 1000000L
#include <util/delay.h>

void init_ad2();
void run_ad2();

int main() {
	run_ad2();
	
	return 0;
}

void init_ad2() {
	DDRA = 0xFF;
	PORTA = 0x00;
	DDRB = 0x00;
	PORTB = 0xFF;
	// Ustawia prescaler zbocza opadajacego, tryb NORMAL
	TCCR0 |= (1 << CS02) | (1 << CS01);
	// Ustawia prescaler zbocza opadajacego, tryb NORMAL
	// TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	TCNT0 = 240;
}

void run_ad2() {
	init_ad2();
	
	uint8_t counter;
	while (1) {
		_delay_ms(200);
		// Odczytuje stan licznika
		counter = TCNT0;
		// Ustawia LEDy w taki sposób aby odzwierciedlaly liczbe nacisniec
		PORTA = counter;
		// Sprawdza stan flagi przepelnienia
		if (TIFR & (1 << TOV0)) {
			// Zatrzymuje timer
			TCCR0 &= (0 << CS02) | (0 << CS01) | (0 << CS00);
		}
	}
}
