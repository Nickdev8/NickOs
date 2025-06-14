section .multiboot
align 4
    dd 0x1BADB002               ; magic number
    dd 0x0                      ; flags
    dd -(0x1BADB002 + 0x0)      ; checksum

section .bss
align 16
stack_bottom:
    resb 4096
stack_top:

section .text
global _start
_start:
    mov esp, stack_top

    extern kernel_main
    call kernel_main

hang:
    jmp hang
