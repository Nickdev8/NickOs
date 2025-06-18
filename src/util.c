#include "vga.h"
#include "util.h"
#include <stddef.h>


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
int strlen(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char *strchr(const char *s, int c) {
    while (*s) {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    return NULL;
}

void *memcpy(void *dest, const void *src, unsigned int n) {
    char *d = (char *)dest;
    const char *ss = (const char *)src;
    for (unsigned int i = 0; i < n; i++)
        d[i] = ss[i];
    return dest;
}
