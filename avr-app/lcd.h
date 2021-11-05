#ifndef LCD_H_
#define LCD_H_

#include "port_config.h"
#include <stdint.h>

void lcd_set_config(struct PortConfig *config);

void lcd_init();

void lcd_move_cursor(uint8_t x, uint8_t y);

void lcd_write(char *chars);

void lcd_clear();

void lcd_number(uint8_t num);

void lcd_home();

#endif /* LCD_H_ */