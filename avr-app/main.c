#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000L

// Okresla stan licznika, przy ktorym powinno dojsc do zapalenia LEDa
// Dla odmierzenia 1 s nale¿y ustawiæ wartoœæ 10.
const uint8_t COUNTER_TOP = 1;
// Licznik przerwañ CTC
volatile uint8_t counter = 0;

/* Obsluguje przerwania wywolane przez Timer 0 w trybie CTC */
ISR(TIMER0_COMP_vect) {
	// Sprawdza, czy licznik osiagnal okreslony stan
	if (counter == COUNTER_TOP) {
		// Zapala LED
		PORTA ^= 1 << PA0;
		// Resetuje licznik
		counter = 0;
	} 
	// Inkrementuje licznik
	counter++;
} 

int main(void) {
	DDRA = 1 << PA0;
	// Ustawia Timer 0 w tryb CTC
	TCCR0 |= (1 << WGM01) | (0 << WGM00);
	// Ustawia preskaler 1024
	TCCR0 |= (1 << CS02) | (1 << CS00);
	// Ustawia liczbe impulsow, po ktorej nastepuje przerwanie
	// Przerwanie ma wystepowac po 0.1 s
	OCR0 = F_CPU / 1024 * 0.1 - 1;// Resetuje stan licznika
	TCNT0 = 0;
	// Aktywuje przerwania Timera 0 w trybie CTC
	TIMSK |= (1 << OCIE0);
	// Aktywuje obsluge przerwan
	sei();
	while (1);
}