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
    // extern "C" void InterruptHandler();

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

        /// @brief Current CPU Registers saved in Stack
        struct CpuState {
            u64 Rax;
            u64 Rbx;
            u64 Rcx;
            u64 Rdx;
            u64 Rbp;
            u64 Rsp;
            u64 Rsi;
            u64 Rdi;
            u64 R8;
            u64 R9;
            u64 R10;
            u64 R11;
            u64 R12;
            u64 R13;
            u64 R14;
            u64 R15;
        } __attribute__((packed));

        /// @brief Interrupt Info pushed to Stack by CPU
        struct StackState {
            u64 ErrorCode;
            u64 Eip;
            u64 CS;
            u64 Eflags;
        } __attribute__((packed));

        struct EC {
            u64 EC;
            u64 EC2;
            u64 EC3;
        } __attribute__((packed));

        static void Register();
        static void UnhandledException(int Code);
    };
}
}

#endif