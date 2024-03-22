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

%macro isr_wrapper 1
isr_wrapper_%+%1:
    call InterruptHandler  ; Call External C++ InterruptHandler
    iretq                  ; Return Instruction 64 bit.
%endmacro

; Define the External InterruptHandler function. Imple
; -mentation present in the interrupt.cpp file. Then, 
; use the NASM preprocessor %rep macro to repeat defining
; a common interrupt wrapper for N interrupts. Additionally,
; define total interrupt count for using them in %rep.

extern InterruptHandler
%assign isr_count 32

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