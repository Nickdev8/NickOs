#include "vga.h"
#include "util.h"
#include "input.h"
#include "ascii_art.h"

void kernel_main() {
    disable_cursor();
    clear_screen();
    print(nick_banner);
    print("> ");
    while (1) {
        if (inb(0x64) & 1) {
            handle_keypress();
        }
    }
}
