global os64load
extern load_kernel

section .text
bits 64

; Entrypoint
os64load:
    ; load 0 into all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call load_kernel
    hlt