#include <avr/io.h>
#define F_CPU 1000000L
#include <util/delay.h>

void init_ad3();
void run_ad3();
void await_overflow();

int main() {
	run_ad3();
	
	return 0;
}

void init_ad3() {
	DDRA = 0xFF;
	PORTA = 0xFF;
	
	// Ustawia prescaler w tryb 1/1024
	TCCR0 |= (1 << CS02) | (1 << CS00);
	
	// Ustawia tryb CTC
	TCCR0 |= (1 << WGM01);
}

void run_ad3() {
	init_ad3();
	
	// Maksymalne wartosci licznika dla poszczególnych czesci okresu
	const uint8_t i150ms = 0.15 * F_CPU / 1024 - 1;
	const uint8_t i50ms = 0.05 * F_CPU / 1024 - 1;
	
	// Resetuje licznik
	TCNT0 = 0;
	
	while(1) {
		// Zmienia maksymalna wartosc licznika
		OCR0 = PORTA ? i150ms : i50ms;
		
		// Opoznia iteracje za pomoca timera
		await_overflow();
		
		// Zmienia stan LEDów
		PORTA ^= 0xff;
	}
}

void await_overflow() {
	// Wstrzymuje program dopoki flaga przepelnienia nie jest ustawiona
	while (!(TIFR & (1 << OCF0)));
	
	// Zdejmuje flage przepelnienia
	TIFR |= (1 << OCF0);
	
	// Ustawia licznik
	TCNT0 = 0;
}
