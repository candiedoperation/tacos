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

#include <kernel/multiboot/mbpvdr.hpp>
#include <kernel/types.hpp>

#define KERNEL_BOOTMEM_PMMGR_BLOCKALLOCLIMIT 512 /* Max 2MB Bitmap Based Alloc */
#define KERNEL_BOOTMEM_PMMGR_BLOCKSPERBYTE 8
#define KERNEL_BOOTMEM_PMMGR_IDETHRESHOLD 5
#define KERNEL_BOOTMEM_PMMGR_BLOCKSIZE 4096
#define KERNEL_BOOTMEM_PMMGR_ALIGN KERNEL_BOOTMEM_PMMGR_BLOCKSIZE
#define KERNEL_BOOTMEM_PMMGR_ALLOCTHRESBYTES (KERNEL_BOOTMEM_PMMGR_IDETHRESHOLD * KERNEL_BOOTMEM_PMMGR_BLOCKSIZE)

#define KERNEL_BOOTMEM_VMMGR_PAGESIZE 4096
#define KERNEL_BOOTMEM_VMMGR_MAPOFFSET 0xffff888000000000

namespace tacOS {
namespace Kernel {
    class BootMem {
    public:
        /* Abstration Types */
        typedef u64 PhysicalAddress;
        typedef u64 VirtualAddress;

        /* Physical Memory Variables */
        static u64 PhysicalFreeBlocks;
        static u64 PhysicalTotalBlocks;
        static u64* PhysicalMemoryMap;

        /* Virtual Memory Variables */
        static u64 VirtualFreePages;
        static u64 VirtualTotalPages;
        static VirtualAddress VirtualMaxIDMappedAddr;

        static inline void PhysicalMemoryMapSet(u64 Bit)
        {
            PhysicalMemoryMap[Bit / 64] |= (1ULL << (Bit % 64));
        }

        static inline void PhysicalMemoryMapUnset(u64 Bit)
        {
            PhysicalMemoryMap[Bit / 64] &= ~(1ULL << (Bit % 64));
        }

        static inline bool PhysicalMemoryMapTest(u64 Bit)
        {
            return PhysicalMemoryMap[Bit / 64] & (1ULL << (Bit % 64));
        }

        static inline u64 AlignAddressToPage(u64 Address)
        {
            return (Address + KERNEL_BOOTMEM_PMMGR_ALIGN - 1) & ~(KERNEL_BOOTMEM_PMMGR_ALIGN - 1);
        }

        static void Initialize();
        static VirtualAddress* VirtAllocateBlock(u64 Size = 1);
        static void VirtFreeBlock(VirtualAddress* AllocatedBlock, u64 Size = 1);

    private:
        static u64 GetPhysicalMemoryMapFreeIndex(u64 Blocks = 1);
        static void InitPhysicalMemory(MBootDef::MemoryMap* MemoryMap);
        static void InitVirtualMemory(MBootDef::MemoryMap* MemoryMap);
        static PhysicalAddress* PhysicalMemoryAllocateBlock(u64 Size = 1);
        static PhysicalAddress* PhysicalMemoryAllocateIDMappedBlock(u64 Size = 1);
        static void PhysicalMemoryFreeBlock(PhysicalAddress* AllocatedBlock, u64 Size = 1);

        static inline void FlushTLBCache() {
            /* Flush Translation Lookaside Buffer (TLB), AT&T Syntax */
            __asm__ volatile(
                "mov %cr3, %rax\n"
                "mov %rax, %cr3"
            );
        }
    };
}
}

#endif