#include "vga.h"
#include "input.h"
#include "shell.h"
#include "util.h"

#define MAX_INPUT 256
#define HISTORY_SIZE 10
#define MAX_COMMAND_LENGTH 256

static char history[HISTORY_SIZE][MAX_COMMAND_LENGTH];
static int history_count = 0;
static int history_index = -1;

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

void redraw_input()
{
    // clear current line
    for (int i = 0; i < VGA_WIDTH; i++)
    {
        print_char_at(' ', col + i, row);
    }

    col = 0;
    print("> ");
    print(input_buffer);
}

void add_to_history(const char *command)
{
    if (command[0] == '\0') return; // skip empty

    if (history_count < HISTORY_SIZE) {
        strcpy(history[history_count++], command);
    } else {
        for (int i = 1; i < HISTORY_SIZE; i++) {
            strcpy(history[i - 1], history[i]);
        }
        strcpy(history[HISTORY_SIZE - 1], command);
    }
    history_index = history_count;
}


void handle_keypress()
{
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80)
    {
        // Key release — ignore
        return;
    }
    if (scancode == 0x48)
    { // UP arrow
        if (history_count == 0)
            return; // nothing to show
        if (history_index > 0)
        {
            history_index--;
            strcpy(input_buffer, history[history_index]);
        }
        input_pos = strlen(input_buffer);
        redraw_input();
        return;
    }
    else if (scancode == 0x50)
    { // DOWN arrow
        if (history_count == 0)
            return; // nothing to do

        if (history_index < history_count - 1)
        {
            history_index++;
            strcpy(input_buffer, history[history_index]);
        }
        else
        {
            // clear buffer and move to empty line
            history_index = history_count;
            input_buffer[0] = '\0';
        }
        input_pos = strlen(input_buffer);
        redraw_input();
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
        add_to_history(input_buffer);
        history_index = history_count;
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
