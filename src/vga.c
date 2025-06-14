#include "vga.h"
#include "util.h"

volatile uint16_t *vga_buffer;
uint8_t current_color;
int row, col;

void init_vga() {
    vga_buffer = (uint16_t *)0xB8000;
    current_color = 0x0F;
    row = 0;
    col = 0;
}

void clear_screen()
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        if (i < VGA_WIDTH * VGA_HEIGHT)
        {
            vga_buffer[i] = ' ' | (current_color << 8);
        }
    }
    col = 0;
    row = 0;
}

void set_color(uint8_t color) {
    current_color = color;
}

void scroll_up()
{
    for (int y = 1; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }
    for (int x = 0; x < VGA_WIDTH; x++)
    {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ' ' | (current_color << 8);
    }
    row = VGA_HEIGHT - 1;
    col = 0;
}

void print(const char *str)
{
    while (*str)
    {
        if (*str == '\n')
        {
            row++;
            col = 0;
        }
        else
        {
            int index = row * VGA_WIDTH + col;
            vga_buffer[index] = (uint8_t)*str | (current_color << 8);
            col++;
            if (col >= VGA_WIDTH)
            {
                col = 0;
                row++;
            }
        }
        if (row >= VGA_HEIGHT)
        {
            scroll_up();
        }
        str++;
    }
}

void backspace()
{
    if (col == 0 && row == 0)
        return;
    if (col == 0)
    {
        row--;
        col = VGA_WIDTH - 1;
    }
    else
    {
        col--;
    }
    int index = row * VGA_WIDTH + col;
    vga_buffer[index] = ' ' | (current_color << 8);
}

void disable_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}
void print_char_at(char c, int x, int y)
{
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT)
    {
        int index = y * VGA_WIDTH + x;
        vga_buffer[index] = (uint8_t)c | (current_color << 8);
    }
}
