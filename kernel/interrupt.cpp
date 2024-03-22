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

#include <asm/io.hpp>
#include <drivers/hal/pic8259.hpp>
#include <drivers/video/vga.hpp>
#include <kernel/interrupt.hpp>

using namespace tacos::Kernel;
using namespace tacos::Drivers::HAL;
using namespace tacos::Drivers::Video;
using namespace tacos::ASM;

namespace tacos {
namespace Kernel {
    void InterruptHandler() {
        __asm__ volatile ("cli; hlt");
    }

    void Interrupt::Register()
    {
        /* Define a Global Interrupt Descriptor Table */
        __attribute__((aligned(0x10))) static IdTableEntry IdTable[256];
        static IdTableRegister Idtr;

        /* IDT Init */
        Idtr.base = (u64)&IdTable[0];
        Idtr.limit = sizeof(IdTable);

        /*
            __asm__ executes traditional assembly block. Does not use GNU Ex
            -tensions or extended assembly. (GCC asm() and asm(:))
            https://wiki.osdev.org/Inline_Assembly/Examples

            __volatile__ is a addon to __asm__ that ensures that compiler
            optimizations do not remove the assembly instructions.
        */

        //__asm__ volatile("lidt %0" : : "m"(Idtr));

        /* Initialize PIC and Set Interrupt Flag */
        Pic8259::Initialize();
        //__asm__ volatile("sti");
    }

    void Interrupt::UnhandledException(int Code)
    {
        /* Define Exception String */
        char* exception = "Unhandled Exception  \n";
        exception[20] = (65 + Code);

        /* Write Exception Output and Halt CPU */
        VgaTextMode::BufferWrite(exception);
        __asm__ __volatile__("cli; hlt");
    }
}
}