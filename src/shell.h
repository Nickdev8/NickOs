#pragma once

typedef void (*CommandFunc)(const char *args);  // accepts args

typedef struct {
    const char *name;
    CommandFunc func;
} Command;

void execute_command(const char *input);  // input = full line, like "theme red"
