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
using namespace tacos::Kernel;

namespace tacos {
    namespace Kernel {
        __attribute__ ((aligned(0x10)))
        static Interrupt::IdTableEntry IdTable[256]; // Static, present till halt
        static Interrupt::IdTableRegister Idtr;

        /* Define Interrupts */
        DEFINE_INTERRUPT(0);
        DEFINE_INTERRUPT(1);
        DEFINE_INTERRUPT(3);
        DEFINE_INTERRUPT(4);
        DEFINE_INTERRUPT(5);
        DEFINE_INTERRUPT(6);
        DEFINE_INTERRUPT(7);
        DEFINE_INTERRUPT(8);
        DEFINE_INTERRUPT(9);
        DEFINE_INTERRUPT(10);
        DEFINE_INTERRUPT(11);
        DEFINE_INTERRUPT(12);
        DEFINE_INTERRUPT(13);
        DEFINE_INTERRUPT(14);
        DEFINE_INTERRUPT(15);
        DEFINE_INTERRUPT(16);
        DEFINE_INTERRUPT(17);
        DEFINE_INTERRUPT(18);
        DEFINE_INTERRUPT(19);
        DEFINE_INTERRUPT(20);
        DEFINE_INTERRUPT(21);
        DEFINE_INTERRUPT(22);
        DEFINE_INTERRUPT(23);
        DEFINE_INTERRUPT(24);
        DEFINE_INTERRUPT(25);
        DEFINE_INTERRUPT(26);
        DEFINE_INTERRUPT(27);
        DEFINE_INTERRUPT(28);
        DEFINE_INTERRUPT(29);
        DEFINE_INTERRUPT(30);
        DEFINE_INTERRUPT(31);
        DEFINE_INTERRUPT(32); 

        void Interrupt::Register() {
            /* IDT Init */
            Idtr.base = (QWORD) &IdTable[0];
            Idtr.limit = sizeof(IdTable);

            IdTableEntry *idt;
            CREATE_INTERRUPT(0);
            CREATE_INTERRUPT(1);
            CREATE_INTERRUPT(3);
            CREATE_INTERRUPT(4);
            CREATE_INTERRUPT(5);
            CREATE_INTERRUPT(6);
            CREATE_INTERRUPT(7);
            CREATE_INTERRUPT(8);
            CREATE_INTERRUPT(9);
            CREATE_INTERRUPT(10);
            CREATE_INTERRUPT(11);
            CREATE_INTERRUPT(12);
            CREATE_INTERRUPT(13);
            CREATE_INTERRUPT(14);
            CREATE_INTERRUPT(15);
            CREATE_INTERRUPT(16);
            CREATE_INTERRUPT(17);
            CREATE_INTERRUPT(18);
            CREATE_INTERRUPT(19);
            CREATE_INTERRUPT(20);
            CREATE_INTERRUPT(21);
            CREATE_INTERRUPT(22);
            CREATE_INTERRUPT(23);
            CREATE_INTERRUPT(24);
            CREATE_INTERRUPT(25);
            CREATE_INTERRUPT(26);
            CREATE_INTERRUPT(27);
            CREATE_INTERRUPT(28);
            CREATE_INTERRUPT(29);
            CREATE_INTERRUPT(30);
            CREATE_INTERRUPT(31);
            CREATE_INTERRUPT(32); 

            __asm__ __volatile__ ("lidt %0" : : "m"(Idtr));
            __asm__ __volatile__ ("sti"); // set the interrupt flag

            VgaTextMode VgaTm;
            VgaTm.BufferWrite("Registered for Exceptions!");
        }

        extern "C" void HandleInterrupt(int Code) {
            char* exception = "Exeception   occured!";
            exception[11] = (48 + Code);

            VgaTextMode VgaTm;
            VgaTm.BufferWrite(exception, VgaColor::WHITE, VgaColor::RED);
        }
    }
}