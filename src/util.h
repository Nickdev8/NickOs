#pragma once
#include <stdint.h>

int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, int n);
int strlen(const char *str);
void strcpy(char *dest, const char *src);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
char *strchr(const char *s, int c);
void *memcpy(void *dest, const void *src, unsigned int n);
