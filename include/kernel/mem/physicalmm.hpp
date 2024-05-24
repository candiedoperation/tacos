/*
    tacOS
    Copyright (C) 2024  Atheesh Thirumalairajan

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

#ifndef KERNEL_PHYSICALMM_HPP
#define KERNEL_PHYSICALMM_HPP

#include <kernel/types.hpp>
#include <kernel/multiboot/mbpvdr.hpp>

#define KERNEL_PHYSICALMM_BLOCKSIZE 4096
#define KERNEL_PHYSICALMM_BLOCKALIGN KERNEL_PHYSICALMM_BLOCKSIZE

namespace tacOS {
namespace Kernel {
    class PhysicalMemory {
    public:
        /// @brief u64 Memory Address
        typedef u64 PhysicalAddress;
        
        /// @brief Memory Statistics
        struct MemoryStats {
            u64 TotalBlocksCount;
            u64 FreeBlocksCount;
            u64 BlockSize;
        };

        static PhysicalAddress* AvailableBlocksPtr;
        static u64 TotalBlocksCount;
        static u64 FreeBlocksCount;
        static u64 TotalMemoryBytes;
        
        static void Initialize();
        static void ProcessMBootMemoryMap(MBootDef::MemoryMap* MemoryMap);
        static PhysicalAddress* AllocateBlock();
        static void FreeBlock(PhysicalAddress* BaseAddress);
        static MemoryStats GetMemoryStatistics();
    };
}
}

#endif