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

#include <kernel/mem/physicalmm.hpp>
#include <tools/kernelrtl/kernelrtl.hpp>

using namespace tacOS::Kernel;
using namespace tacOS::Tools::KernelRTL;

/// @brief Kernel End Linker Label
extern void* KRNL_END;

/* Define Statics */
PhysicalMemory::PhysicalAddress* PhysicalMemory::AvailableBlocksPtr;
u64 PhysicalMemory::TotalBlocksCount;
u64 PhysicalMemory::FreeBlocksCount;
u64 PhysicalMemory::TotalMemoryBytes;

/// @brief Initialization Routine for Physical Memory Manager
void PhysicalMemory::Initialize()
{
    /*
        This routine initializes the Physical Memory Management
        component of the kernel. We're using a stack-based appr
        -oach so that we have a O(1) complexity for both alloc
        -ation and de-allocation.

        Refer:
        http://www.osdever.net/tutorials/view/memory-management-1
        https://forum.osdev.org/viewtopic.php?f=1&t=33727
    */

    /* Align Stack to 4KB Mark from Kernel End */
    u64 StackAddress = ((((u64)&KRNL_END) & ~(KERNEL_PHYSICALMM_BLOCKALIGN - 1)) + (u64)&KRNL_END);
    AvailableBlocksPtr = (PhysicalAddress*)StackAddress;

    /* Populate Stack using Multiboot Memory Map */
    MBootDef::MemoryMap* MBootMemoryMap = MBootProvider::MemoryMapPtr;
    ProcessMBootMemoryMap(MBootMemoryMap);
}

/// @brief Allocates a block of Physical Memory
/// @return Pointer to the allocated block of Memory
PhysicalMemory::PhysicalAddress* PhysicalMemory::AllocateBlock() {
    if (FreeBlocksCount < 1)        
        return 0;
    
    /* Pop the Address off the stack */
    PhysicalAddress AllocatedBlock = *(--AvailableBlocksPtr);
    *AvailableBlocksPtr = 0;
    FreeBlocksCount--;
    
    /* Return Pointer to Allocated Address */
    return (PhysicalAddress*) AllocatedBlock;
}

/// @brief Frees a block of Physical Memory
/// @param BaseAddress Pointer to an existing block of Memory
void PhysicalMemory::FreeBlock(PhysicalAddress* BaseAddress) {
    *(AvailableBlocksPtr++) = (u64) BaseAddress;
    FreeBlocksCount++;
}

/// @brief Process Multiboot Memory Map Entry
/// @param MemoryMap Pointer to Multiboot2 Memory Map Entry
void PhysicalMemory::ProcessMBootMemoryMap(MBootDef::MemoryMap* MemoryMap)
{
    /* FUTURE: USE DEBUG STATEMENTS? */
    printf("\nBIOS Memory Map: ");

    for (
        MBootDef::MemoryMapEntry* MMapEntry = (MBootDef::MemoryMapEntry*)(MemoryMap + 1);
        ((u8*)MMapEntry) - ((u8*)(MemoryMap + 1)) < (MemoryMap->Header.Size - sizeof(MBootDef::MemoryMap));
        MMapEntry = (MBootDef::MemoryMapEntry*)((u8*)MMapEntry + MemoryMap->EntrySize)) {

        printf("\n    Region Start: 0x");
        printf(MMapEntry->BaseAddress, 16);
        printf(", Length: ");
        printf(MMapEntry->Length / 1024);
        printf("KB ");

        switch (MMapEntry->Type) {
        case MBootDef::MemoryMapEntryType::AVAILABLE: {
            /* Update Number of Available Blocks */
            u64 DiscoveredBlocks = (MMapEntry->Length / KERNEL_PHYSICALMM_BLOCKSIZE);
            FreeBlocksCount += DiscoveredBlocks;
            TotalBlocksCount += DiscoveredBlocks;

            for (u64 Offset = 0; Offset < DiscoveredBlocks; Offset++) {
                /* Push Available Memory Locations to Stack */
                *AvailableBlocksPtr++ = (MMapEntry->BaseAddress + Offset);
            }

            printf("(Available)");
            break;
        }

        case MBootDef::MemoryMapEntryType::ACPI_INFO:
            printf("(ACPI Information)");
            break;

        case MBootDef::MemoryMapEntryType::RESERVED:
            printf("(Reserved)");
            break;

        case MBootDef::MemoryMapEntryType::HIBERNATE:
            printf("(Hibernate Info)");
            break;

        case MBootDef::MemoryMapEntryType::DEFECTIVE:
            printf("(Defective)");
            break;

        default:
            printf(MMapEntry->Type);
            break;
        }
    }

    /* Looks Nice? ;-) */
    printf("\n");
}