/*
    tacOS
    Copyright (C) 2023  Atheesh Thirumalairajan

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <kernel/interrupt.hpp>
#include <drivers/video/vga.hpp>

using namespace tacos::Drivers::Video;

namespace tacos {
    namespace Kernel {
        //static IdTableEntry IdTable[256]; // Static, present till halt
        //static IdTableRegister Idtr;

        void Interrupt::Register() {
            __attribute__((aligned(0x10)))
            static IdTableEntry IdTable[256];
            static IdTableRegister Idtr;

            void* ptr = (void *) &IntDivByZero;

            /* IDT Init */
            Idtr.base = (QWORD) &IdTable[0];
            Idtr.limit = (sizeof(IdTable) - 1) << 16;

            /* Update the first Idtr */
            IdTableEntry *idtr1 = &IdTable[0];
            idtr1->IsrLow = ((QWORD) ptr) & 0xFFFF;
            idtr1->KernelCs = 0x08;
            idtr1->Ist = 0;
            idtr1->Attributes = 0x8e;
            idtr1->IsrMid = ((QWORD) ptr >> 16) & 0xFFFF;
            idtr1->IsrHigh = ((QWORD) ptr >> 32) & 0xFFFFFFFF;
            idtr1->Reserved = 0;

            asm volatile ("lidt %0" : : "m"(Idtr));
            asm volatile ("sti"); // set the interrupt flag

            VgaTextMode VgaTm;
            VgaTm.BufferWrite("Registered for Exceptions!");
        }

        void Interrupt::IntDivByZero() {
            VgaTextMode VgaTm;
            VgaTm.BufferWrite("Div By Zero Exception!");
        }
    }
}