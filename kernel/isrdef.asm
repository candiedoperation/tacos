;   tacOS
;   Copyright (C) 2024  Atheesh Thirumalairajan
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


; This Assembly File contains:
; Interrupt Service Routine (ISR) Definitions.

; Define Assmebler Macros to define routines
; for all possible interrupt codes.
;
; macro write here. 
;
; Refer:
; https://wiki.osdev.org/Interrupts_tutorial#ISRs
; https://github.com/cstack/osdev/blob/master/asm_interrupts.s

%macro isr_wrapper 1
isr_wrapper_%+%1:

    ; Providing parameters to CPP functions in long mode:
    ; After mutliple failed attempts of using the 'push'
    ; instruction in x64, I resorted to manually moving
    ; the parameters to the registers.
    ;
    ; In x86-64 calling convention, the first six integer or 
    ; pointer parameters are passed in registers (rdi, rsi, 
    ; rdx, rcx, r8, r9), and any additional parameters are 
    ; passed on the stack.
    ;
    ; Example:
    ; mov rdi, 66             ; First parameter
    ; mov rsi, 65             ; Second parameter
    ; mov rdx, 64             ; Third parameter
    ; mov rcx, 63             ; Fourth parameter
    ; mov r8, 62              ; Fifth parameter
    ; mov r9, 61              ; Sixth parameter
    ; sub rsp, 40             ; Allocate space for additional parameters on the stack
    ;
    ; mov qword [rsp], 60     ; Seventh parameter
    ; mov qword [rsp + 8], 59 ; Eighth parameter
    ;
    ; add rsp, 40             ; Restore the stack pointer after the call

    mov rdi, %+%1          ; Interrupt Code Parameter
    sub rsp, 8             ; Align the stack
    call InterruptHandler  ; Call External C++ InterruptHandler
    add rsp, 8             ; Restore the stack pointer (u64 is 8 bytes)

    ; Return from Interrupt (64 bit instruction).
    iretq
%endmacro

; Define the External InterruptHandler function. Imple
; -mentation present in the interrupt.cpp file. Then, 
; use the NASM preprocessor %rep macro to repeat defining
; a common interrupt wrapper for N interrupts. Additionally,
; define total interrupt count for using them in %rep.

extern InterruptHandler ; Matches with interrupt.hpp
%assign isr_count 50    ; Should Match with interrupt.hpp

%assign ihctr 0
%rep isr_count
    isr_wrapper ihctr
    %assign ihctr ihctr + 1
%endrep

; Define a Global ISR Table that has memory locations
; to every isr_wrapper block as defined above. This
; table is global such that its accessible from C++.
;
; Populate the table using the %rep NASM preprocessor 
; instruction to populate the table with 64 bit long
; memory locations. Use dq to define 64 bit mem locs.
;
; Refer:
; https://wiki.osdev.org/Interrupts_tutorial#ISRs

global IsrWrapperTable
IsrWrapperTable:
%assign ctr 0
%rep isr_count
    dq isr_wrapper_%+ctr
    %assign ctr ctr+1
%endrep