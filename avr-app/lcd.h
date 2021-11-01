#include <stdint.h>

#ifndef LCD_H_
#define LCD_H_

void lcd_init();

void lcd_move_cursor(uint8_t x, uint8_t y);

void lcd_write(char *chars);

void lcd_clear();


#endif /* LCD_H_ */