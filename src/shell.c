#include "shell.h"
#include "vga.h"
#include "util.h"
#include "ascii_art.h"
#include "util.h"
#include "fs.h"


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

// list cwd
void cmd_ls(const char *args) {
    (void)args;
    fs_list();
}

// mkdir <name>
void cmd_mkdir(const char *args) {
    if (!*args) {
        print("Usage: mkdir <name>\n");
        return;
    }
    if (fs_mkdir(args) == 0)
        print("OK\n");
    else
        print("Error\n");
}

// cd <name|..>
void cmd_cd(const char *args) {
    if (!*args) {
        print("Usage: cd <dir>\n");
        return;
    }
    if (fs_chdir(args) != 0)
        print("No such directory\n");
}

// cat <file>
void cmd_cat(const char *args) {
    if (!*args) {
        print("Usage: cat <file>\n");
        return;
    }
    fs_node_t *f = fs_open(args, 0);
    if (!f || f->type != FS_FILE) {
        print("No such file\n");
        return;
    }
    char buf[128];
    unsigned int n;
    while ((n = fs_read(f, buf, sizeof(buf))) > 0) {
        buf[n] = 0;
        print(buf);
    }
        print("\n");
}

// write <file> <text>
void cmd_write(const char *args) {
    // split filename and text
    const char *text = strchr(args, ' ');
    if (!text) {
        print("Usage: write <file> <text>\n");
        return;
    }
    char name[FS_MAX_NAME];
    unsigned int len = (unsigned int)(text - args);
    if (len >= FS_MAX_NAME) len = FS_MAX_NAME-1;
    memcpy(name, args, len);
    name[len] = 0;
    text++; // skip space
    fs_node_t *f = fs_open(name, 1);
    if (!f) {
        print("Error opening file\n");
        return;
    }
    fs_write(f, text, strlen(text));
    print("OK\n");
}


Command commands[] = {
    { "help", cmd_help },
    { "clear", cmd_clear },
    { "about", cmd_about },
    { "theme", cmd_theme },
    { "ls",    cmd_ls    },
    { "mkdir", cmd_mkdir },
    { "cd",    cmd_cd    },
    { "cat",   cmd_cat   },
    { "write", cmd_write },
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
