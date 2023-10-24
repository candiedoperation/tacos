global start

section .multiboot
mboot_start:
    dd 0xe85250d6 ; Multiboot Magic #
    dd 0          ; i386 Protected Mdoe
    dd mboot_end - mboot_start ; Header Length
    dd 0x100000000 - (0xe85250d6 + 0 + (mboot_end - mboot_start)) ; Checksum

    ; Required End Flags
    dw 0 ; Type
    dw 0 ; Flags
    dd 8 ; Size
mboot_end:

section .data
    msg_head db "-------[ osloader.asm ]-------", 0
    msg_init db "* Initialization OK", 0
    msg_check db "* Checking CPU Capabilities", 0
    msg_verified db "* CPU Check Passed", 0
    msg_error db "CPU Error ", 0
    msg_call db "* Loading tacOS Kernel", 0

section .text
bits 32 ; Set CPU to 32 Bit Protected Mode

; Entrypoint
start:
    push 0x0f
    push 0x000b8000
    push msg_head
    call print

    push 0x0f
    push 0x000b8140
    push msg_init
    call print

    push 0x0f
    push 0x000b81e0
    push msg_check
    call print

    ; Check CPU Capabilities
    call cpu_check

    ; CPU Check Successful
    push 0x0f
    push 0x000b8280
    push msg_verified
    call print

    push 0x0a
    push 0x000b8320
    push msg_call
    call print

    hlt

; Error
error:
    push 0x0c
    push 0x000b8280
    push msg_error
    call print

    mov bh, 0x0c
    mov dword [0x000b8294], ebx
    hlt

; CPU Check - Tests for 64-bit Long Mode
cpu_check:
    call check_mboot
    call check_cpuid
    call check_lm
    ret
    
; Multiboot Check
check_mboot:
    cmp eax, 0x36d76289
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