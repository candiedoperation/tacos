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
    extern "C" void* IsrWrapperTable[];
    extern "C" void InterruptHandler(u64 InterruptCode)
    {
        if (InterruptCode == 32) {
            VgaTextMode::BufferWrite(".", VgaTextMode::Color::GREEN, VgaTextMode::Color::BLACK);
            Pic8259::EndOfInterrupt(32);
        } else {
            VgaTextMode::BufferWrite("Exception!");
            __asm__ volatile ("cli; hlt");
        }
    }

    void Interrupt::Register()
    {
        /* Define a Global Interrupt Descriptor Table */
        __attribute__((aligned(0x10))) static IdTableEntry IdTable[256];
        static IdTableRegister Idtr;

        /* IDT Init */
        Idtr.base = (u64)&IdTable[0];
        Idtr.limit = sizeof(IdTable);

        /* Populate the Interrupt Descriptor Table */
        for (u8 Offset = 0; Offset < INTERRUPT_ISRCOUNT; Offset++) {
            IdTableEntry* Idt = &IdTable[Offset];
            Idt->IsrLow = ((u64)IsrWrapperTable[Offset]) & 0xFFFF;
            Idt->KernelCs = 0x08; /* What's GDT_OFFSET_KERNEL_CODE? */
            Idt->Ist = 0;
            Idt->Attributes = 0x8E; /* What's this Flag? */
            Idt->IsrMid = ((u64)IsrWrapperTable[Offset] >> 16) & 0xFFFF;
            Idt->IsrHigh = ((u64)IsrWrapperTable[Offset] >> 32) & 0xFFFFFFFF;
            Idt->Reserved = 0;
        }

        /* Initialize Interrupt Controllers */
        Pic8259::Initialize();

        /*
            __asm__ executes traditional assembly block and it does
            not use GNU Extensions or extended assembly. (GCC asm()
            and asm(:)). Furthermore, __volatile__ is an addon to
            __asm__ that ensures that compiler optimizations do not
            remove the assembly instructions.

            Refer:
            https://wiki.osdev.org/Inline_Assembly/Examples
        */

        __asm__ volatile("lidt %0" : : "m"(Idtr));
        __asm__ volatile("sti");
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