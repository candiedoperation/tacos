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

#ifndef KERNEL_BOOTMEM_HPP
#define KERNEL_BOOTMEM_HPP

#include <kernel/types.hpp>
#include <kernel/multiboot/mbpvdr.hpp>

#define KERNEL_BOOTMEM_PMMGR_BLOCKALLOCLIMIT 512 /* Max 2MB Bitmap Based Alloc */
#define KERNEL_BOOTMEM_PMMGR_BLOCKSPERBYTE 8
#define KERNEL_BOOTMEM_PMMGR_BLOCKSIZE 4096
#define KERNEL_BOOTMEM_PMMGR_ALIGN KERNEL_BOOTMEM_PMMGR_BLOCKSIZE

namespace tacOS {
namespace Kernel {
    class BootMem {
    private:
        static u64 GetPhysicalMemoryMapFreeIndex(u64 Blocks = 1);
        static void InitializePhysicalMemory(MBootDef::MemoryMap* MemoryMap);
        static void InitializeVirtualMemory();

    public:
        /* Memory Address Abstractions */
        typedef u64 PhysicalAddress;
        typedef u64 VirtualAddress;
        
        /* Physical Memory Specifics */
        static u64 PhysicalFreeBlocks;
        static u64 PhysicalTotalBlocks;
        static u64* PhysicalMemoryMap;

        static inline void PhysicalMemoryMapSet(u64 Bit)
        {
            PhysicalMemoryMap[Bit / 64] |= (1 << (Bit % 64));
        }

        static inline void PhysicalMemoryMapUnset(u64 Bit)
        {
            PhysicalMemoryMap[Bit / 64] &= ~(1 << (Bit % 64));
        }

        static inline bool PhysicalMemoryMapTest(u64 Bit)
        {
            return PhysicalMemoryMap[Bit / 64] & (1 << (Bit % 64));
        }

        static inline u64 AlignAddressToPage(u64 Address)
        {
            return (Address + KERNEL_BOOTMEM_PMMGR_ALIGN - 1) & ~(KERNEL_BOOTMEM_PMMGR_ALIGN - 1);
        }

        static void Initialize();
        static PhysicalAddress* PhysicalMemoryAllocateBlock(u64 Size = 1);
        static void PhysicalMemoryFreeBlock(PhysicalAddress* AllocatedBlock, u64 Size = 1);
        static VirtualAddress* VirtAllocateBlock();
        static void VirtFreeBlock(VirtualAddress* AllocatedBlock);
    };
}
}

#endif