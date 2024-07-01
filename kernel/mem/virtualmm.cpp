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
#include <kernel/mem/bootmem.hpp>
#include <kernel/mem/virtualmm.hpp>
#include <tools/kernelrtl/kernelrtl.hpp>

using namespace tacOS::Kernel;

/* osloader.asm Page Tables */
extern VirtualMemory::PML4Table osloader_pml4t;

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

/// @brief Maps a Physical Memory Address to the Virtual Address Space
/// @param BaseAddress Physical Memory Address (Pointer)
/// @return Pointer to the mapped Virtual Address
VirtualMemory::VirtualAddress* VirtualMemory::MapPhysicalFrame(
    PhysicalMemory::PhysicalAddress* BaseAddress)
{
}

/// @brief Maps Physical Memory Mapped Hardware Addresses to Virtual Address Space
/// @param BaseAddress Memory-Mapped Physical Address
/// @return Memory-Mapped Virtual Address
VirtualMemory::VirtualAddress* VirtualMemory::HardwareRemap(
    PhysicalMemory::PhysicalAddress* BaseAddress)
{
    /* UGLY CODE !!!, TEMPORARY */
    /* FUTURE: Impl. should use Virtual Address Manager, Include Security!!! */
    BootMem::PhysicalMemoryMapToOffset((u64)BaseAddress, KERNEL_VIRTMM_HWMEM_MAPOFFSET);
    return (VirtualAddress*)((u64)BaseAddress + KERNEL_VIRTMM_HWMEM_MAPOFFSET);
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
}