#include <avr/io.h>
#include <avr/interrupt.h>

/* Obsluguje przerwanie INT0 */
ISR(INT0_vect) {
	PORTA ^= 1 << PA0;
}

/* Obsluguje przerwanie INT2 */
ISR(INT2_vect) {
	PORTA ^= 1 << PA2;
}

/* Przyjmuje wartosc MCUCR i zwraca z ustawionym
wyzwalaniem INT0 na zboczach opadajaych */
uint8_t INT0_falling(uint8_t bits) {
	bits &= ~(1 << ISC00);
	bits |= 1 << ISC01;
	return bits;
}

/* Przyjmuje wartosc MCUCR i zwraca z ustawionym
wyzwalaniem INT0 na zboczach narastajacych */
uint8_t INT0_raising(uint8_t bits) {
	return bits | 1 << ISC00 | 1 << ISC01;
}

int main(void) {
	DDRA = 1 << PA0 | 1 << PA2;
	
	PORTB = 1 << PB2;
	PORTD = 1 << PD2;
	
	cli();
	
	// Ustawia wyzwalanie INT0 INT2 na zboczach narastajacych
	MCUCR = INT0_raising(MCUCR);
	MCUCSR |= 1 << ISC2;
	
	// Ustawia wyzwalanie INT0 INT2 na zboczach opadajacych
	// MCUCR = INT0_falling(MCUCR);
	// MCUCSR &= 0 << ISC2;
	
	// Aktywuje przerwania INT0 INT2
	GICR |= 1 << INT0 | 1 << INT2;
	GIFR |= 1 << INT0 | 1 << INT2;
	
	// Aktywuje obsluge przerwan
	sei();
	
	while (1);
}