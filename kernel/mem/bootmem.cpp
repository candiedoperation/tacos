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

#include <kernel/mem/bootmem.hpp>
#include <kernel/mem/virtualmm.hpp>
#include <kernel/mem/physicalmm.hpp>
#include <kernel/assert/logging.hpp>
#include <tools/kernelrtl/kernelrtl.hpp>

using namespace tacOS::Kernel;
using namespace tacOS::Tools::KernelRTL;

/// @brief Kernel End Linker Label
extern void* KRNL_END;

/* Define Statics */
u64 BootMem::PhysicalFreeBlocks;
u64 BootMem::PhysicalTotalBlocks;
u64* BootMem::PhysicalMemoryMap;

void BootMem::Initialize()
{
    /*
        The initialization routine involves initializing a minimal
        Bitmap-based Physical Memory Manager. A stack-based approach
        is not possible initially as only the first 2MB of the kernel
        is identity mapped and a stack would overflow that size leading
        to page faults while setting up paging.

        This routine initializes Physical Memory Management for atmost
        2MB of physical memory. Furthermore, it sets up preliminary paging
        to allow for allocation of dynamic structures. Once this is complete,
        the actual Physical and Virtual Memory Managers take over and map
        the available memory completely and manage the address space thereafter.

        Refer:
        http://www.brokenthorn.com/Resources/OSDev17.html
    */

    /* Initialize the Physical Memory Map (Bitmap Based) */
    u64 BitmapAddress = AlignAddressToPage((u64)&KRNL_END);
    u64 MBootInfoEndAddr = ((u64)MBootProvider::MBootInfoPtr) + MBootProvider::MBootInfoPtr->Length;

    /* Check if Multiboot Structure is After Chosen Address */
    if (MBootInfoEndAddr > BitmapAddress)
        BitmapAddress = AlignAddressToPage(MBootInfoEndAddr + 1);

    /* Allocate Address to Physical Memory Map */
    PhysicalMemoryMap = (u64*)(BitmapAddress);

    /* Populate Memory Information using the Multiboot Memory Map */
    MBootDef::MemoryMap* MBootMemoryMap = MBootProvider::MemoryMapPtr;
    PopulateMBootMemoryInfo(MBootMemoryMap);

    /* Log Initialization */
    Logging::LogMessage(Logging::LogLevel::DEBUG, "Bootmem Allocator Init Complete");
}

/// @brief Allocate a Block of Physical Memory
/// @return Pointer to Allocated Block
BootMem::PhysicalAddress* BootMem::PhysicalMemoryAllocateBlock()
{
    /* Get First Free Location, Check if Out of Memory */
    u64 Frame = GetPhysicalMemoryMapFreeIndex();
    if (Frame == -1)
        return 0;

    /* Set Frame Allocated, Update Free Frames */
    PhysicalMemoryMapSet(Frame);
    PhysicalFreeBlocks--;

    /* Return Physical Address */
    return (PhysicalAddress*)(Frame * KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);
}

/// @brief Gets the Next Free Physical Memory Location from Bitmap
/// @return Physical Memory Address
u64 BootMem::GetPhysicalMemoryMapFreeIndex()
{
    /* Check if Out of Memory */
    if (PhysicalFreeBlocks < 1)
        return -1;

    for (u64 i = 0; i < (PhysicalTotalBlocks / 64); i++) {
        /* Check if Block isn't Full */
        if (PhysicalMemoryMap[i] != 0xFFFFFFFFFFFFFFFF) {
            /* Check each bit in Block to find an Empty Address */
            for (u8 j = 0; j < 64; j++) {
                u64 Bit = 1ULL << j;
                if (!(PhysicalMemoryMap[i] & Bit))
                    return (i * 64) + j;
            }
        }
    }

    /* Out of Memory! */
    return -1;
}

void BootMem::PopulateMBootMemoryInfo(MBootDef::MemoryMap* MemoryMap)
{
    for (
        MBootDef::MemoryMapEntry* MMapEntry = (MBootDef::MemoryMapEntry*)(MemoryMap + 1);
        ((u8*)MMapEntry) - ((u8*)(MemoryMap + 1)) < (MemoryMap->Header.Size - sizeof(MBootDef::MemoryMap));
        MMapEntry = (MBootDef::MemoryMapEntry*)((u8*)MMapEntry + MemoryMap->EntrySize)) {

        /* Get Discovered Blocks */
        u64 DiscoveredBlocks = (MMapEntry->Length / KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);
        u64 AlignedBaseAddr = (MMapEntry->BaseAddress / KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);
        PhysicalTotalBlocks += DiscoveredBlocks;

        if (MMapEntry->Type == MBootDef::MemoryMapEntryType::AVAILABLE) {
            /* Update Free Blocks Count and Mark As Available */
            PhysicalFreeBlocks += DiscoveredBlocks;
            for (; DiscoveredBlocks > 0; DiscoveredBlocks--) {
                PhysicalMemoryMapUnset(AlignedBaseAddr++);
            }

            /* Prevent Alloc at 0x00 */
            PhysicalMemoryMapSet(0);
        } else {
            /* Update Unavailable Blocks */
            for (; DiscoveredBlocks > 0; DiscoveredBlocks--) {
                PhysicalMemoryMapSet(AlignedBaseAddr++);
            }
        }
    }
}