global start

section .multiboot
mboot_start:
    dd 0xe85250d6                                    ; Multiboot Magic #
    dd 0                                             ; i386 Protected Mdoe
    dd mboot_end - mboot_start                       ; Header Length
    dd 0x100000000 - (0xe85250d6 + 0 + (mboot_end - mboot_start)) ; Checksum

    ; Required End Flags
    dw 0 ; Type
    dw 0 ; Flags
    dd 8 ; Size
mboot_end:

section .text
bits 32 ; Set CPU to 32 Bit Protected Mode
start:
    mov dword [0xb8000], 0x2f4b2f4f
    hlt