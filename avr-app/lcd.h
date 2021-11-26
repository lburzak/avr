#ifndef LCD_H_
#define LCD_H_

#include "port_config.h"
#include <stdint.h>

// Ustawia port na ktorym ma dzialac LCD
void lcd_set_config(struct PortConfig *config);

// Inicjalizuje wyswietlacz
void lcd_init();

// Przenosi kursor do wybranej pozycji
void lcd_move_cursor(uint8_t x, uint8_t y);

// Wypisuje ciag znakow zaczynajac od obecnej pozycji
void lcd_write(char *chars);

// Czysci caly ekran
void lcd_clear();

// Wypisuje dwucyfrowa liczbe
void lcd_number(uint8_t num);

// Przenosi kursor na poczatek ekranu
void lcd_home();

// Usuwa znaki od wybranej pozycji
void lcd_clear_from(uint8_t x, uint8_t y);

#endif /* LCD_H_ */