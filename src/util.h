#pragma once
#include <stdint.h>

int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, int n);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
