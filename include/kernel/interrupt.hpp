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

namespace tacos {
    namespace Kernel {
        class Interrupt {
            struct IdTableEntry {
                WORD IsrLow;
                WORD KernelCs;
                BYTE Ist;
                BYTE Attributes;
                WORD IsrMid;
                DWORD IsrHigh;
                DWORD Reserved;
            };

            struct IdTableRegister {
                WORD limit;
                QWORD base;
            };

            /* Define Interrupt Descriptor Table */
            static IdTableEntry IdTable[256]; // Static, present till halt
            static IdTableRegister Idtr;

            public:
                void Register();
                static void IntDivByZero();
        };
    }
}

#endif