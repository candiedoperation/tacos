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
        #define DEFINE_INTERRUPT(x) extern "C" void HandleInterrupt##x() { HandleInterrupt(x); }
        #define CREATE_INTERRUPT(x) \
            idt = &IdTable[x]; \
            idt->IsrLow = ((QWORD) &HandleInterrupt##x) & 0xFFFF; \
            idt->KernelCs = 0x08; \
            idt->Ist = 0; \
            idt->Attributes = 0x8e; \
            idt->IsrMid = ((QWORD) &HandleInterrupt##x >> 16) & 0xFFFF; \
            idt->IsrHigh = ((QWORD) &HandleInterrupt##x >> 32) & 0xFFFFFFFF; \
            idt->Reserved = 0;

        /* Define Main Interrupt Handler */
        extern "C" void HandleInterrupt(int Code);

        class Interrupt {
            public:
                struct IdTableEntry {
                    WORD IsrLow;
                    WORD KernelCs;
                    BYTE Ist;
                    BYTE Attributes;
                    WORD IsrMid;
                    DWORD IsrHigh;
                    DWORD Reserved;
                } __attribute__((packed));

                struct IdTableRegister {
                    WORD limit;
                    QWORD base;
                } __attribute__((packed));
                
                void Register();
        };
    }
}

#endif