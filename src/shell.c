#include "vga.h"
#include "ascii_art.h"
#include "util.h"

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
    else
    {
        print("Unknown command. Type 'help'\n");
    }
}