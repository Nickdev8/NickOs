#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
uint16_t* vga_buffer = (uint16_t*) 0xB8000;

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (uint8_t)' ' | (0x0F << 8);  // space with white on black
    }
}

void print(const char* str) {
    static int row = 0, col = 0;
    while (*str) {
        if (*str == '\n') {
            row++;
            col = 0;
        } else {
            int index = row * VGA_WIDTH + col;
            vga_buffer[index] = (uint8_t)*str | (0x0F << 8);  // ASCII + color
            col++;
        }
        str++;
    }
}

void kernel_main() {
    clear_screen();
    print("NickOS Terminal v0.0.1\n> ");
    while (1) {
        __asm__("hlt");
    }
}
