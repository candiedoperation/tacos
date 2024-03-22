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

extern InterruptHandler
isr_wrapper 0
isr_wrapper 1
isr_wrapper 2
isr_wrapper 3
isr_wrapper 4
isr_wrapper 5
isr_wrapper 6
isr_wrapper 7
isr_wrapper 8
isr_wrapper 9
isr_wrapper 10
isr_wrapper 11
isr_wrapper 12
isr_wrapper 13
isr_wrapper 14
isr_wrapper 15
isr_wrapper 16
isr_wrapper 17
isr_wrapper 18
isr_wrapper 19
isr_wrapper 20
isr_wrapper 21
isr_wrapper 22
isr_wrapper 23
isr_wrapper 24
isr_wrapper 25
isr_wrapper 26
isr_wrapper 27
isr_wrapper 28
isr_wrapper 29
isr_wrapper 30
isr_wrapper 31

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
%rep 32
    dq isr_wrapper_%+ctr
    %assign ctr ctr+1
%endrep