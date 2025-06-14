#pragma once
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern volatile uint16_t *vga_buffer;
extern uint8_t current_color;
extern int row, col;

void print(const char *str);
void clear_screen();
void scroll_up();
void backspace();
void disable_cursor();
