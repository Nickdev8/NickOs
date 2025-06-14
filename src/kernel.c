#include "vga.h"
#include "util.h"
#include "input.h"
#include "ascii_art.h"

void kernel_main()
{
    init_vga();

    disable_cursor();
    clear_screen();
    outb(0x3D4, 0x0B);
    outb(0x3D5, 0x0F);

    print(nick_banner);
    print("> ");
    while (1)
    {
        if (inb(0x64) & 1)
        {
            handle_keypress();
        }
    }
}
