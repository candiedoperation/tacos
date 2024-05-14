;   tacOS
;   Copyright (C) 2023  Atheesh Thirumalairajan
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this program.  If not, see <https://www.gnu.org/licenses/>.

global osload
global mboot_ebx
extern os64load

section .multiboot
mboot_start:
    dd 0xe85250d6 ; Multiboot Magic #
    dd 0          ; i386 Protected Mdoe
    dd mboot_end - mboot_start ; Header Length
    dd 0x100000000 - (0xe85250d6 + 0 + (mboot_end - mboot_start)) ; Checksum

    ; Multiboot Struture to Enable Linear Framebuffers
    ;dw 5
    ;dw 0
    ;dd 20
    ;dd 0
    ;dd 0
    ;dd 0
    ;align 8

    ; Required End Flags
    dw 0 ; Type
    dw 0 ; Flags
    dd 8 ; Size
mboot_end:

section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64 ; new
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .data
    msg_head db "-------[ osloader.asm ]-------", 0
    msg_error db "CPU Error ", 0
    mboot_eax dd 0
    mboot_ebx dd 0

section .bss
align 4096
pml4_table:
    resb 4096
pdp_table:
    resb 4096
pd_table:
    resb 4096
page_table:
    resb 4096

section .text
bits 32 ; Set CPU to 32 Bit Protected Mode

; Entrypoint
osload:
    ; Save Multiboot Pointers
    call mboot_cache

    push 0x0f
    push 0x000b8000
    push msg_head
    call print

    ; Check CPU Capabilities
    call cpu_check

    ; Enable Long Mode
    call memory_paging
    call enable_paging

    ; Load GDT, Perform Long Jump
    lgdt [gdt64.pointer]
    jmp gdt64.code:os64load
    hlt

; Error
error:
    push 0x0c
    push 0x000b80a0
    push msg_error
    call print

    mov bh, 0x0c
    mov dword [0x000b80b4], ebx
    hlt

; Cache Multiboot Struct
mboot_cache:
    mov [mboot_eax], eax
    mov [mboot_ebx], ebx
    ret

; CPU Check - Tests for 64-bit Long Mode
cpu_check:
    call check_mboot
    call check_cpuid
    call check_lm
    ret
    
; Multiboot Check
check_mboot:
    cmp dword [mboot_eax], 0x36d76289
    jne error_mboot
    ret

error_mboot:
    mov ebx, 0x41
    jmp error

; CPUID Check
check_cpuid:
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax

    ; Copy to ECX as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
    cmp eax, ecx
    je error_cpuid
    ret

error_cpuid:
    mov ebx, 0x42
    jmp error

; Long Mode Check
check_lm:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb error_lm           ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz error_lm           ; If it's not set, there is no long mode
    ret

error_lm:
    mov ebx, 0x43
    jmp error

; Setup Basic Paging
memory_paging:
    ; Map PDP Table to first PML4 Entry
    mov edx, pdp_table ; Move Memory Address of pdp_table to edx
    or edx, 0b11 ; Present + Writable bits
    mov [pml4_table], edx ; Move edx to pml4_table's first index

    ; Map PD Table to first PDP Table Entry
    mov edx, pd_table
    or edx, 0b11
    mov [pdp_table], edx

    ; Map each PD Table entry to a 2MB Page
    mov ecx, 0
    map_pd:
        mov eax, 0x200000
        mul ecx ; ecx -> source, eax -> destination (implied)
        or eax, 0b10000011 ; present + writable + huge
        mov [pd_table + (ecx * 8)], eax

        inc ecx
        cmp ecx, 512
        jne map_pd
    
    enable_paging:
    ; Load PML4 Table to CR3 Register
    mov eax, pml4_table
    mov cr3, eax

    ; Enable Physical Address Extension (PAE) using the CR4 Register
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set Long Mode bit in Model Specific Register
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable Paging in CR0 Register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Return memory_paging
    ret

; Print
print:
    push ebp
    mov ebp, esp

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]

    print_loop:
        mov edx, [esi]
        test dl, dl
        jz end_loop
        mov dh, [ebp + 16]
        mov dword [edi], edx

        inc esi
        inc edi
        inc edi
        jmp print_loop

    end_loop:
        mov esi, 0
        mov edi, 0

    pop ebp
    ret