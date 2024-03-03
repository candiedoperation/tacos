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

#include <drivers/hal/pic8259.hpp>
#include <drivers/video/vga.hpp>
#include <kernel/interrupt.hpp>

using namespace tacos::Kernel;
using namespace tacos::Drivers::HAL;
using namespace tacos::Drivers::Video;

namespace tacos {
namespace Kernel {
    __attribute__((aligned(0x10))) static Interrupt::IdTableEntry IdTable[256]; // Static, present till halt
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

    /* Define PIC Interrupts */
    DEFINE_INTERRUPT(32);
    DEFINE_INTERRUPT(33);
    DEFINE_INTERRUPT(34);
    DEFINE_INTERRUPT(35);
    DEFINE_INTERRUPT(36);
    DEFINE_INTERRUPT(37);
    DEFINE_INTERRUPT(38);
    DEFINE_INTERRUPT(39);
    DEFINE_INTERRUPT(40);
    DEFINE_INTERRUPT(41);
    DEFINE_INTERRUPT(42);
    DEFINE_INTERRUPT(43);
    DEFINE_INTERRUPT(44);
    DEFINE_INTERRUPT(45);
    DEFINE_INTERRUPT(46);
    DEFINE_INTERRUPT(47);

    void Interrupt::Register()
    {
        /* IDT Init */
        Idtr.base = (u64)&IdTable[0];
        Idtr.limit = sizeof(IdTable);

        IdTableEntry* idt;
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

        /* Create PIC Interrupts */
        CREATE_INTERRUPT(32);
        CREATE_INTERRUPT(33);
        CREATE_INTERRUPT(34);
        CREATE_INTERRUPT(35);
        CREATE_INTERRUPT(36);
        CREATE_INTERRUPT(37);
        CREATE_INTERRUPT(38);
        CREATE_INTERRUPT(39);
        CREATE_INTERRUPT(40);
        CREATE_INTERRUPT(41);
        CREATE_INTERRUPT(42);
        CREATE_INTERRUPT(43);
        CREATE_INTERRUPT(44);
        CREATE_INTERRUPT(45);
        CREATE_INTERRUPT(46);
        CREATE_INTERRUPT(47);

        /*
            __asm__ executes traditional assembly block. Does not use GNU Ex
            -tensions or extended assembly. (GCC asm() and asm(:))
            https://wiki.osdev.org/Inline_Assembly/Examples

            __volatile__ is a addon to __asm__ that ensures that compiler
            optimizations do not remove the assembly instructions.
        */

        __asm__ volatile("lidt %0" : : "m"(Idtr));

        /* Initialize PIC and Set Interrupt Flag */
        Pic8259::Initialize();
        __asm__ volatile("sti");

        VgaTextMode::BufferWrite("Registered for Exceptions!\n", vga_color::WHITE, vga_color::GREEN);
    }

    extern "C" void HandleInterrupt(int Code)
    {
        switch (Code) {
        case 0:
            Interrupt::HandleDivByZeroException();
            break;

        case 8:
            Interrupt::HandleDoubleFaultException();
            break;

        case 14:
            Interrupt::HandlePageFaultException();
            break;

        case 32:
            VgaTextMode::BufferWrite(".");
            Pic8259::EndOfInterrupt(Code);
            break;

        default:
            Interrupt::UnhandledException(Code);
            break;
        }
    }

    void Interrupt::HandleDivByZeroException()
    {
        VgaTextMode::BufferWrite("Divide By Zero Exception!\n");
    }

    void Interrupt::HandleDoubleFaultException()
    {
        VgaTextMode::BufferWrite("Double Fault Occured!");
        __asm__ __volatile__("cli; hlt");
    }

    void Interrupt::HandlePageFaultException()
    {
        VgaTextMode::BufferWrite("Page Fault Exception!");
        __asm__ __volatile__("cli; hlt");
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