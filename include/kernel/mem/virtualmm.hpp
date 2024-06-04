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

#ifndef KERNEL_VIRTMEM_PAGEMAP_HPP
#define KERNEL_VIRTMEM_PAGEMAP_HPP

#include <kernel/types.hpp>

#define KERNEL_VIRTMM_PAGESIZE 4096
#define KERNEL_VIRTMM_MAXPML4E 512
#define KERNEL_VIRTMM_MAXPDPTE 512
#define KERNEL_VIRTMM_MAXPDE 512
#define KERNEL_VIRTMM_MAXPTE 512
#define KERNEL_VIRTMM_PAGEMASK 0x1FF /* 0001 1111 1111 (9 bits) */

/* Virtual Address Space Offsets */
#define KERNEL_VIRTMM_PHYMEM_MAPOFFSET 0xffff888000000000
#define KERNEL_VIRTMM_PHYMEM_MAPOFFSETEND 0xffffc87fffffffff

namespace tacOS {
namespace Kernel {
    /// @brief Defines Routines and Structures for Virtual Memory Management
    class VirtualMemory {
    public:
        typedef u64 VirtualAddress;
        typedef u64 PTEntry;
        typedef u64 PDEntry;
        typedef u64 PDPEntry;
        typedef u64 PML4Entry;

        /// @brief PML4 Table Entry Flags
        enum struct PML4EntryFlags : u64 {
            PRESENT = 1 << 0,
            WRITABLE = 1 << 1,
            USERSPACE = 1 << 2,
            PLVL_WRITETHROUGH = 1 << 3,
            CACHE_DISABLE = 1 << 4,
            ACCESSED = 1 << 5,
            EXECUTE_DISABLED = ((u64) 1) << 63
        };

        /// @brief PDP Table Entry Flags
        enum struct PDPTEntryFlags : u64 {
            PRESENT = 1 << 0,
            WRITABLE = 1 << 1,
            USERSPACE = 1 << 2,
            PLVL_WRITETHROUGH = 1 << 3,
            CACHE_DISABLE = 1 << 4,
            ACCESSED = 1 << 5,
            HUGEPAGE = 1 << 7, /* 1GiB Pages */
            EXECUTE_DISABLED = ((u64) 1) << 63
        };

        /// @brief Page Directory Entry Flags
        enum struct PDEntryFlags : u64 {
            PRESENT = 1 << 0,
            WRITABLE = 1 << 1,
            USERSPACE = 1 << 2,
            PLVL_WRITETHROUGH = 1 << 3,
            CACHE_DISABLE = 1 << 4,
            ACCESSED = 1 << 5,
            HUGEPAGE = 1 << 7, /* 2MiB Pages */
            EXECUTE_DISABLED = ((u64) 1) << 63
        };

        /// @brief Page Table Entry Flags
        enum struct PTEntryFlags : u64 {
            PRESENT = 1 << 0,
            WRITABLE = 1 << 1,
            USERSPACE = 1 << 2,
            WRITETHROUGH = 1 << 3,
            ACCESSED = 1 << 4,
            DIRTY = 1 << 5,
            PAT = 1 << 6,
            GLOBAL = 1 << 7,

            /* Available for Operating System Use (Bits 9-11) */
            OS_AVL1 = 1 << 8,
            OS_AVL2 = 1 << 9,
            OS_AVL3 = 1 << 10,

            /* HLAT RESET, ignored for normal paging (Bit 12) */
            HLAT_RST = 1 << 11
        };

        struct PML4Table {
            PML4Entry Entries[KERNEL_VIRTMM_MAXPML4E];
        };

        struct PDPTable {
            PDPEntry Entries[KERNEL_VIRTMM_MAXPDPTE];
        };

        struct PDTable {
            PDEntry Entries[KERNEL_VIRTMM_MAXPDE];
        };

        struct PTable {
            PTEntry Entries[KERNEL_VIRTMM_MAXPTE];
        };

        struct PTableEx {
            PTEntry Entries[1024];
        };

        static inline u64 GetPML4Index(VirtualAddress VirtAddress) {
            return ((VirtAddress >> 39) & KERNEL_VIRTMM_PAGEMASK);
        }

        static inline u64 GetPDPTIndex(VirtualAddress VirtAddress) {
            return ((VirtAddress >> 30) & KERNEL_VIRTMM_PAGEMASK);
        }

        static inline u64 GetPDTIndex(VirtualAddress VirtAddress) {
            return ((VirtAddress >> 21) & KERNEL_VIRTMM_PAGEMASK);
        }
        
        static inline u64 GetPTIndex(VirtualAddress VirtAddress) {
            return ((VirtAddress >> 12) & KERNEL_VIRTMM_PAGEMASK);
        }

        static VirtualAddress* AllocateBlock(PML4Table* PML4TablePtr);
        static void Intialize();
    };
}
}

#endif