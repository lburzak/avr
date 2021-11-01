#define F_CPU 1000000
#include "lcd.h"
#include <util/delay.h>

int main(void) {	
	lcd_init();
	lcd_clear();
	lcd_move_cursor(1, 3);
	lcd_write("hello");
}

