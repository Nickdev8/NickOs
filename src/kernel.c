#include <stdint.h>

#define VGA_WIDTH 160
#define VGA_HEIGHT 50
uint16_t* vga_buffer = (uint16_t*) 0xB8000;

#define MAX_INPUT 256
char input_buffer[MAX_INPUT];
int input_pos = 0;

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (uint8_t)' ' | (0x0F << 8);  // space with white on black
    }
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char scancode_to_ascii[128] = {
  0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b', // Backspace
 '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n', // Enter
  0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
  0, '\\','z','x','c','v','b','n','m',',','.','/', 0, '*',
  0, ' ', // Spacebar
  // rest are unused
};

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
int strcmp(const char* a, const char* b) {
    while (*a && *b && *a == *b) {
        a++; b++;
    }
    return *a - *b;
}


void execute_command(const char* cmd) {
    if (strcmp(cmd, "help") == 0) {
        print("Commands: help, clear, about\n");
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "about") == 0) {
        print("NickOS - Tiny DIY Terminal\n");
    } else {
        print("Unknown command. Type 'help'\n");
    }
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void handle_keypress() {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        // Key release — ignore
        return;
    }

    char key = scancode_to_ascii[scancode];
    if (!key) return;

    if (key == '\b') {
        if (input_pos > 0) {
            input_pos--;
            print("\b \b"); // erase char
        }
        return;
    }

    if (key == '\n') {
        input_buffer[input_pos] = '\0';
        print("\n");
        execute_command(input_buffer);
        input_pos = 0;
        print("> ");
        return;
    }

    if (input_pos < MAX_INPUT - 1) {
        input_buffer[input_pos++] = key;
        char str[2] = {key, 0};
        print(str);
    }
}

void kernel_main() {
    disable_cursor();
    clear_screen();
    print("NickOS Terminal v0.0.1\n> ");

    while (1) {
        if (inb(0x64) & 1) {
            handle_keypress();
        }
    }
}
