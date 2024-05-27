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

#include <kernel/assert/logging.hpp>
#include <kernel/mem/physicalmm.hpp>
#include <kernel/mem/virtualmm.hpp>
#include <tools/kernelrtl/kernelrtl.hpp>

using namespace tacOS::Kernel;

/* osloader.asm Page Tables */
extern VirtualMemory::PML4Table osloader_pml4t;

/* Define Statics */
static VirtualMemory::PML4Table PML4Table0 alignas(4096);
static VirtualMemory::PDPTable PDPTable0;
static VirtualMemory::PDTable PDTable0;
static VirtualMemory::PTable PTable0;
static VirtualMemory::PTable PTable1;

/// @brief
/// @param PML4Table
/// @return
VirtualMemory::VirtualAddress* VirtualMemory::AllocateBlock(PML4Table* PML4TablePtr)
{
    /* Allocate a Physical Memory Block */
    PhysicalMemory::PhysicalAddress PhyAddress
        = (PhysicalMemory::PhysicalAddress)(PhysicalMemory::AllocateBlock());

    if (PhyAddress == 0)
        return 0;

    VirtualAddress VirtAddress = (PhyAddress + KERNEL_VIRTMM_PHYMEM_MAPOFFSET);
    Tools::KernelRTL::printf("Physical Address 0x");
    Tools::KernelRTL::printf(PhyAddress, 16);
    Tools::KernelRTL::printf(" -> Virtual Address 0x");
    Tools::KernelRTL::printf(VirtAddress, 16);
}

/// @brief Setup Structures, Configure Memory Paging
void VirtualMemory::Intialize()
{
    /*
        Paging in long mode consists of four maps - page directory pointer table,
        page directory, page table, and the level-4 page map table (PML4). This
        routine intializes the required virtual address space. Firstly, the first
        2MB of physical address space is identity mapped then the kernel is mapped
        to the higher-half of the virtual address space (128TiB+). Lastly, the CR3
        register is loaded with the new page table.

        Refer:
        https://wiki.osdev.org/Paging
        https://wiki.osdev.org/Page_Tables
        https://wiki.osdev.org/Setting_Up_Paging
        http://www.brokenthorn.com/Resources/OSDev18.html
        http://www.osdever.net/tutorials/view/memory-management-1
    */

    /* Identity Map first 2MB of Physical Memory */
    for (int i = 0; i < KERNEL_VIRTMM_MAXPTE; i++) {
        PTable0.Entries[i] = (i * KERNEL_VIRTMM_PAGESIZE) | 3;
    }

    /* 1g is identify mapped by osloader.asm */
    PDTable0.Entries[0] = ((u64) &PTable0) | 3;
    PDPTable0.Entries[0] = ((u64) &PDTable0) | 3;
    PML4Table0.Entries[0] = ((u64) &PDPTable0) | 3;
    
    /* Directly Map 2MB of Available Physical Memory */
    for (int i = 0; i < KERNEL_VIRTMM_MAXPTE; i++) {
        //PhysicalMemory::AvailableBlocksPtr
    }

    __asm__ volatile ("mov %0, %%cr3" : : "r" (&PML4Table0) : "memory");
    //__asm__ volatile("mov %0, %%cr3" : : "r"(&osloader_pml4t) : "memory");
}