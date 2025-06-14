#include "ascii_art.h"
#include "util.h"

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
const char *get_ascii(const char *name)
{
    if (strcmp(name, "cat") == 0)
        return ascii_cat;
    if (strcmp(name, "rocket") == 0)
        return ascii_rocket;
    return 0;
}
