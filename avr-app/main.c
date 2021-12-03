#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000L

// Zmienna w SRAM, do przechowywania stanu przyciskow
volatile uint8_t buttons = 0;

// Obsluguje przerwania wywolane przez Timer 0 w trybie CTC
ISR(TIMER0_COMP_vect) {
	// Przechowuje stan przyciskow (nieistotne bity sa odrzucone)
	buttons = PINA & 0x0f;
}

int main(void) {
	DDRA = 0x00;
	DDRC = 0xff;
	
	// Ustawia Timer 0 w tryb CTC
	TCCR0 |= (1 << WGM01) | (0 << WGM00);
	
	// Ustawia preskaler 64
	TCCR0 |= (1 << CS01) | (1 << CS00);
	
	// Ustawia liczbe impulsow, po ktorej nastepuje przerwanie// Przerwanie ma wystepowac po 0.005 s
	OCR0 = F_CPU / 64 * 0.005 - 1;
	
	// Resetuje stan licznika
	TCNT0 = 0;
	
	// Aktywuje przerwania Timera 0 w trybie CTC
	TIMSK |= (1 << OCIE0);
	
	// Aktywuje obsluge przerwan
	sei();
	
	// Petla glowna wyswietla stan przyciskow na LEDach
	while (1) PORTC = buttons;
}