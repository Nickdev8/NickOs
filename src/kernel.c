#include "vga.h"
#include "util.h"
#include "input.h"
#include "ascii_art.h"

void kernel_main() {
    disable_cursor();         // Try commenting this out
    // clear_screen();           // Try commenting this out
    print("Boot test\n");        // Keep this
    print(nick_banner);       // Maybe corrupt data?
    print("> ");
    while (1) {
        if (inb(0x64) & 1) {
            handle_keypress();
        }
    }
}
