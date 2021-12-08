#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#define F_CPU 1000000L

// Zmienna w SRAM, do przechowywania stanu przyciskow
volatile uint8_t buttons = 0;

// Obsluguje przerwania wywolane przez Timer 0 w trybie CTC
ISR(TIMER0_COMP_vect) {
	// Zapisuje kod wcisnietego przycisku do zmiennej globalnej
	buttons = keypad_read();
}

int main(void) {
	DDRA = 0xf0;
	PORTA = 0xff;
	
	// LEDy zostaly podlaczone do portu B, zamiast C ze wzgledu na wade stykow plytki mikrokontrolera na zajeciach
	DDRB = 0xff;
	
	// Ustawia port klawiatury
	struct PortConfig keypad_config = {
		.DDR = &DDRA,
		.PIN = &PINA,
		.PORT = &PORTA
	};
	
	keypad_set_config(&keypad_config);
	
	// Ustawia Timer 0 w tryb CTC
	TCCR0 |= (1 << WGM01) | (0 << WGM00);
	
	// Ustawia preskaler 64
	TCCR0 |= (1 << CS01) | (1 << CS00);
	
	// Ustawia liczbe impulsow, po ktorej nastepuje przerwanie
	// Przerwanie ma wystepowac po 0.005 s
	OCR0 = F_CPU / 64 * 0.005 - 1;
	
	// Resetuje stan licznika
	TCNT0 = 0;
	
	// Aktywuje przerwania Timera 0 w trybie CTC
	TIMSK |= (1 << OCIE0);
	
	// Aktywuje obsluge przerwan
	sei();
	
	// Petla glowna wyswietla kod przycisku na LEDach
	while (1) PORTB = buttons;
}