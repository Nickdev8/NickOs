#pragma once

typedef void (*CommandFunc)(const char *args);  // accepts args

typedef struct {
    const char *name;
    CommandFunc func;
} Command;

void execute_command(const char *input);  // input = full line, like "theme red"


// void cmd_theme(const char *args);
// void cmd_ls(const char *args);
// void cmd_mkdir(const char *args);
// void cmd_cd(const char *args);
// void cmd_cat(const char *args);
// void cmd_write(const char *args);