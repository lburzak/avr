#include <avr/io.h>
#define F_CPU 1000000L
#include <util/delay.h>

void delay_10ms();
void delay_1s();
void init_ad1();
void run_ad1();

int main() {
	run_ad1();
	
	return 0;
}

void init_ad1() {
	DDRA = 0xFF;
	PORTA = 0x00;
	
	// Ustawia prescaler w tryb 1/256
	TCCR0 |= (1 << CS02);
	
	// Ustawia tryb CTC
	TCCR0 |= (1 << WGM01);
	
	// Ustawia maksymalna wartosc licznika
	OCR0 = 0.01 * F_CPU / 256 - 1;
	
	// Resetuje licznik
	TCNT0 = 0;
}

void run_ad1() {
	init_ad1();
	
	while(1) {
		// Opoznia iteracje o 1 sekunde
		delay_1s();
		// Przelacza LED
		PORTA ^= (1 << PA0);
	}
}

void delay_10ms() {
	while (!(TIFR & (1 << OCF0)));
	
	// Zdejmuje flage przepelnienia
	TIFR |= (1 << OCF0);
	
	// Resetuje licznik
	TCNT0 = 0;
}

void delay_1s() {
	for (uint8_t i = 0; i < 100; i++)
		delay_10ms();
}