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

#ifndef KERNEL_INTERRUPT_HPP
#define KERNEL_INTERRUPT_HPP

#include <kernel/types.hpp>

using namespace tacos::Kernel;

#define INTERRUPT_ISRCOUNT 50 /* Should match with isrdef.asm */

namespace tacos {
namespace Kernel {
    /* Define Interrupt Handler and Table Defined in ASM */
    //extern "C" void InterruptHandler();

    class Interrupt {
    public:
        struct IdTableEntry {
            u16 IsrLow;
            u16 KernelCs;
            u8 Ist;
            u8 Attributes;
            u16 IsrMid;
            u32 IsrHigh;
            u32 Reserved;
        } __attribute__((packed));

        struct IdTableRegister {
            u16 limit;
            u64 base;
        } __attribute__((packed));

        static void Register();
        static void UnhandledException(int Code);
    };
}
}

#endif