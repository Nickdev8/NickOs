#pragma once
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern volatile uint16_t *vga_buffer;
extern uint8_t current_color;
extern int row, col;

void print(const char *str);
void clear_screen();
void set_color(uint8_t color);
void scroll_up();
void backspace();
void disable_cursor();
void print_char_at(char c, int x, int y);
void init_vga();