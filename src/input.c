#include "vga.h"
#include "input.h"
#include "shell.h"
#include "util.h"

#define MAX_INPUT 256

char input_buffer[MAX_INPUT];
int input_pos = 0;

char scancode_to_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // Backspace
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  // Enter
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 0,
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', // Spacebar
            // rest are unused
};

void handle_keypress()
{
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80)
    {
        // Key release — ignore
        return;
    }

    char key = scancode_to_ascii[scancode];
    if (!key)
        return;

    if (key == '\b')
    {
        if (input_pos > 0)
        {
            input_pos--;
            backspace();
        }
        return;
    }

    if (key == '\n')
    {
        input_buffer[input_pos] = '\0';
        print("\n");
        execute_command(input_buffer);
        input_pos = 0;
        print("> ");
        return;
    }

    if (input_pos < MAX_INPUT - 1)
    {
        input_buffer[input_pos++] = key;
        char str[2] = {key, 0};
        print(str);
    }
}