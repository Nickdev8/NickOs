#include "shell.h"
#include "vga.h"
#include "util.h"
#include "ascii_art.h"


void cmd_help(const char *args) {
    (void)args;
    print("Commands: help, clear, about, theme <color>\n");
}

void cmd_clear(const char *args) {
    (void)args;
    clear_screen();
}

void cmd_about(const char *args) {
    (void)args;
    print("NickOS - Tiny DIY Terminal\nMade for HackClub Neighberhood");
}

void cmd_theme(const char *args) {
    if (!args || args[0] == 0) {
        print("Usage: theme <color>\n");
        return;
    }
    if (strcmp(args, "red") == 0) {
        set_color(0x4);
        print("Theme set to red\n");
    } else if (strcmp(args, "green") == 0) {
        set_color(0x2);
        print("Theme set to green\n");
    } else {
        print("Unknown theme color\n");
    }
}

Command commands[] = {
    { "help", cmd_help },
    { "clear", cmd_clear },
    { "about", cmd_about },
    { "theme", cmd_theme },
};

int command_count = sizeof(commands) / sizeof(Command);

void execute_command(const char *input) {
    if (!input || input[0] == 0) return;

    const char *space = input;
    while (*space && *space != ' ') {
        space++;
    }

    int name_len = space - input;

    for (int i = 0; i < command_count; i++) {
        const char *cmd = commands[i].name;

        int match = 1;
        for (int j = 0; j < name_len; j++) {
            if (cmd[j] != input[j]) {
                match = 0;
                break;
            }
        }

        if (match && cmd[name_len] != '\0') {
            match = 0;
        }

        if (match) {
            while (*space == ' ') space++;
            commands[i].func(*space ? space : "");
            return;
        }
    }

    print("Unknown command: ");
    print(input);
    print("\n");
}
