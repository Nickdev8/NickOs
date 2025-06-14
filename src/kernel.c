#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
uint16_t *vga_buffer = (uint16_t *)0xB8000;

#define MAX_INPUT 256
char input_buffer[MAX_INPUT];
int input_pos = 0;
int row = 0, col = 0;
uint8_t current_color = 0x0F; // white on black

const char *nick_banner =
    " _   _ _      _       ____   ____  \n"
    "| \\ | (_) ___| |__   / ___| |  _ \\ \n"
    "|  \\| | |/ __| '_ \\  \\___ \\ | | | |\n"
    "| |\\  | | (__| | | |  ___) || |_| |\n"
    "|_| \\_|_|\\___|_| |_| |____(_)____/ \n";
const char *ascii_cat =
    " /\\_/\\\n"
    "( o.o )\n"
    " > ^ <\n";

const char *ascii_rocket =
    "   ^\n"
    "  /^\\\n"
    "  |#|\n"
    " /###\\\n"
    "|#####|\n"
    " |###|\n"
    "  |||\n";

void clear_screen()
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        vga_buffer[i] = (uint8_t)' ' | (current_color << 8);
    }
    col = 0;
    row = 0;
}
void scroll_up()
{
    // Move all lines up by one
    for (int y = 1; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }

    // Clear last line
    for (int x = 0; x < VGA_WIDTH; x++)
    {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ' ' | (current_color << 8);
    }

    row = VGA_HEIGHT - 1;
    col = 0;
}
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char scancode_to_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // Backspace
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  // Enter
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', // Spacebar
            // rest are unused
};
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

int strcmp(const char *a, const char *b)
{
    while (*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return *a - *b;
}
int strncmp(const char *a, const char *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (a[i] != b[i] || a[i] == '\0' || b[i] == '\0')
            return a[i] - b[i];
    }
    return 0;
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

void execute_command(const char *cmd)
{
    if (strcmp(cmd, "help") == 0)
    {
        print("Commands: help, clear, about\n");
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        clear_screen();
    }
    else if (strcmp(cmd, "about") == 0)
    {
        print("NickOS - Tiny DIY Terminal\n");
    }
    else if (strncmp(cmd, "ascii ", 6) == 0)
    {
        const char *name = cmd + 6;
        if (strcmp(name, "cat") == 0)
            print(ascii_cat);
        else if (strcmp(name, "rocket") == 0)
            print(ascii_rocket);
        else
            print("Unknown ASCII art\n");
    }
    else if (strncmp(cmd, "theme ", 6) == 0)
    {
        const char *color = cmd + 6;
        if (strcmp(color, "white") == 0)
            current_color = 0x0F;
        else if (strcmp(color, "red") == 0)
            current_color = 0x0C;
        else if (strcmp(color, "green") == 0)
            current_color = 0x0A;
        else if (strcmp(color, "blue") == 0)
            current_color = 0x09;
        else if (strcmp(color, "hacker") == 0)
            current_color = 0x0A; // green on black
        else
            print("Unknown theme. Try white, red, green, blue, hacker\n");

        print("> ");
    }
    else
    {
        print("Unknown command. Type 'help'\n");
    }
}

void disable_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

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

void kernel_main()
{
    disable_cursor();
    clear_screen();
    print(nick_banner);
    // print("NickOS Terminal v0.0.1\n> ");
    print("> ");

    while (1)
    {
        if (inb(0x64) & 1)
        {
            handle_keypress();
        }
    }
}
