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
#include <kernel/mem/physicalmm.hpp>
#include <kernel/mem/virtualmm.hpp>
#include <tools/kernelrtl/kernelrtl.hpp>

using namespace tacOS::Kernel;
using namespace tacOS::Tools::KernelRTL;

/* Kernel Linker Labels */
extern void* KRNL_START;
extern void* KRNL_END;

/* Define Statics */
u64 BootMem::PhysicalFreeBlocks;
u64 BootMem::PhysicalTotalBlocks;
u64* BootMem::PhysicalMemoryMap;

u64 BootMem::VirtualFreePages;
u64 BootMem::VirtualTotalPages;
u64 BootMem::VirtualMaxIDMappedAddr;

/* OSLoader Paging Tables */
extern VirtualMemory::PML4Table osloader_pml4t;
extern VirtualMemory::PDPTable osloader_pdpt;
extern VirtualMemory::PDTable osloader_pdt;
extern VirtualMemory::PTable osloader_ptbl;

void BootMem::Initialize()
{
    /*
        The initialization routine involves initializing a minimal
        Bitmap-based Physical Memory Manager. A stack-based approa
        -ch is not possible initially as only the first 2MB of the
        kernel is identity mapped and a stack would overflow that
        size leading to page faults while setting up paging.

        Furthermore, the routine maps all available physical memory
        addresses to an offset in the Virtual Memory Address space.
        Once this is complete, the actual Physical and Virtual Memory
        Managers take over and manage the address space thereafter.

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

    /* OSLoader Maps the first Page Table (2MB) */
    VirtualTotalPages = KERNEL_VIRTMM_MAXPTE;
    VirtualMaxIDMappedAddr = VirtualMemory::GetBaseAddress(osloader_ptbl.Entries[511]);

    /* Populate Memory Information using the Multiboot Memory Map */
    MBootDef::MemoryMap* MBootMemoryMap = MBootProvider::MemoryMapPtr;
    InitPhysicalMemory(MBootMemoryMap);

    // int i = 1000;
    // while (i--) {
    //     printf("\nAlloc: 0x");
    //     printf((u64)PhysicalMemoryAllocateBlock(), 16);
    // }

    InitVirtualMemory(MBootMemoryMap);

    printf("\n\nPhysical Memory: ");
    printf((PhysicalTotalBlocks - PhysicalFreeBlocks) * 4);
    printf("KB Used, ");
    printf(PhysicalFreeBlocks * 4);
    printf("KB Available of ");
    printf(PhysicalTotalBlocks * 4);
    printf("KB");

    /* Log Initialization */
    Logging::LogMessage(Logging::LogLevel::DEBUG, "Bootmem Allocator Init Complete");
}

/// @brief Allocate a Block of Physical Memory
/// @param Size Required number of contiguous blocks
/// @return Pointer to Allocated Block
BootMem::PhysicalAddress* BootMem::PhysicalMemoryAllocateBlock(u64 Size)
{
    /* Get First Free Location, Check if Out of Memory */
    u64 Frame = GetPhysicalMemoryMapFreeIndex(Size);
    if (Frame == -1)
        return 0;

    /* Set Frames Allocated */
    for (u64 i = 0; i < Size; i++) {
        PhysicalMemoryMapSet(Frame + i);
    }

    /* Clear the Memory Block */
    PhysicalAddress* BaseAddress = (PhysicalAddress*)(Frame * KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);
    memset(BaseAddress, 0, (Size * KERNEL_BOOTMEM_PMMGR_BLOCKSIZE));

    /* Update Free Frames and Return Physical Address */
    PhysicalFreeBlocks -= Size;
    return BaseAddress;
}

/// @brief Allocates a block of Identity-mapped Physical Memory
/// @param Size Required number of contiguous blocks
/// @return Pointer to Allocated Block
BootMem::PhysicalAddress* BootMem::PhysicalMemoryAllocateIDMappedBlock(u64 Size)
{
    PhysicalAddress* AllocBase = PhysicalMemoryAllocateBlock(Size);
    PhysicalAddress* AllocOffset = AllocBase + (Size * KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);

    /* Check if Addr is in the Identity Map Region */
    if ((u64)AllocOffset <= VirtualMaxIDMappedAddr)
        return AllocBase;

    else {
        /* Return the Block if block is >4KB then, Get a new one */
        PhysicalAddress* AllocatedPT;
        if (Size == 1)
            AllocatedPT = AllocBase;

        else {
            PhysicalMemoryFreeBlock(AllocBase, Size);
            AllocatedPT = PhysicalMemoryAllocateBlock(1);
        }

        /* Get the Next Page (Not Identity Mapped) */
        VirtualAddress IDMapBaseAddr = (VirtualMaxIDMappedAddr + KERNEL_VIRTMM_PAGESIZE);
        u64 PML4Index = VirtualMemory::GetPML4Index(IDMapBaseAddr);
        u64 PDPTIndex = VirtualMemory::GetPDPTIndex(IDMapBaseAddr);
        u64 PDTIndex = VirtualMemory::GetPDTIndex(IDMapBaseAddr);

        printf("\n=> IDME -> PML4 Index: ");
        printf(PML4Index);
        printf(", PDPT Index: ");
        printf(PDPTIndex);
        printf(", PDT Index: ");
        printf(PDTIndex);
        printf(", PT Index: ");
        printf(VirtualMemory::GetPTIndex(IDMapBaseAddr));

        /* Get Pointers to the Page Tables */
        VirtualMemory::PML4Table* PDPTable = (VirtualMemory::PML4Table*)VirtualMemory::GetBaseAddress(osloader_pml4t.Entries[PML4Index]);
        VirtualMemory::PDTable* PDTable = (VirtualMemory::PDTable*)VirtualMemory::GetBaseAddress(PDPTable->Entries[PDPTIndex]);

        /* WERE NOT CHECKING PDT FOR NOW. FATAL IF ID MAP EXTENDS 1GB */
        // if (!love) { printf("I'm at UMD") }

        /* Limit Identity Maps to 512G */
        if (PML4Index > 1)
            return 0;

        /* Fill the New Page Table */
        for (int i = 0; i < KERNEL_VIRTMM_MAXPTE; i++) {
            AllocatedPT[i] = IDMapBaseAddr | 3;
            IDMapBaseAddr += KERNEL_VIRTMM_PAGESIZE;
        }

        /* Map Page Table to appropriate Tables */
        PDTable->Entries[PDTIndex] = ((u64) AllocatedPT) | 3;

        /* Update Max Addr, Flush the TLB (Translation Lookaside Buffer) */
        VirtualMaxIDMappedAddr = AllocatedPT[KERNEL_VIRTMM_MAXPTE - 1];
        __asm__ volatile("mov %0, %%cr3" : : "r"(&osloader_pml4t) : "memory");

        /* Reallocate the required Block */
        AllocBase = PhysicalMemoryAllocateBlock(Size);
        return AllocBase;
    }
}

/// @brief Frees an Allocated Block of Memory
/// @param AllocatedBlock Pointer to Allocated Block
void BootMem::PhysicalMemoryFreeBlock(PhysicalAddress* AllocatedBlock, u64 Size)
{
    /* Process Block Addresses */
    u64 BaseAddress = (u64)AllocatedBlock;
    u64 Frame = (BaseAddress / KERNEL_PHYSICALMM_BLOCKSIZE);

    /* Mark as Free, Update Free Blocks */
    PhysicalFreeBlocks += Size;
    for (u64 i = 0; i < Size; i++) {
        PhysicalMemoryMapUnset(Frame + i);
    }
}

/// @brief Gets the Next Free Physical Memory Location from Bitmap
/// @return Physical Memory Address
u64 BootMem::GetPhysicalMemoryMapFreeIndex(u64 Blocks)
{
    /* Check if Out of Memory */
    if (PhysicalFreeBlocks < Blocks)
        return -1;

    for (u64 i = 0; i < (PhysicalTotalBlocks / 64); i++) {
        /* Check if Block isn't Full */
        if (PhysicalMemoryMap[i] != 0xFFFFFFFFFFFFFFFF) {
            /* Check each bit in Block to find an Empty Address */
            for (u8 j = 0; j < 64; j++) {
                u64 Bit = 1ULL << j;
                if (!(PhysicalMemoryMap[i] & Bit)) {
                    /* Check if requested Block Length is available */
                    u64 FreeBlocks = 0;
                    u64 SearchIndex = (i * 64) + j;

                    for (u64 k = 0; k < Blocks; k++) {
                        if (!PhysicalMemoryMapTest(SearchIndex + k))
                            FreeBlocks++;

                        else
                            break;

                        if (FreeBlocks == Blocks)
                            return SearchIndex;
                    }
                }
            }
        }
    }

    /* Out of Memory! */
    return -1;
}

void BootMem::InitPhysicalMemory(MBootDef::MemoryMap* MemoryMap)
{
    /* Stores Base Address of Memory Region Processed */
    u64 PrevBitmapIndex = 0;

    for (
        MBootDef::MemoryMapEntry* MMapEntry = (MBootDef::MemoryMapEntry*)(MemoryMap + 1);
        ((u8*)MMapEntry) - ((u8*)(MemoryMap + 1)) < (MemoryMap->Header.Size - sizeof(MBootDef::MemoryMap));
        MMapEntry = (MBootDef::MemoryMapEntry*)((u8*)MMapEntry + MemoryMap->EntrySize)) {

        /* Get Discovered Blocks */
        u64 DiscoveredBlocks = (MMapEntry->Length / KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);
        PhysicalTotalBlocks += DiscoveredBlocks;

        /* Store Bitmap Index and Base Memory Address */
        u64 BaseAddress = MMapEntry->BaseAddress;
        u64 OffsetAddress = AlignAddressToPage(BaseAddress + MMapEntry->Length);
        u64 BitmapIndex = (BaseAddress / KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);

        /* Map Undefined Blocks between Previous End Address and Current Base */
        while (PrevBitmapIndex++ < BitmapIndex) {
            PhysicalMemoryMapSet(PrevBitmapIndex);
        }

        /* Update Free Blocks Count and Mark As Available */
        while (BaseAddress <= OffsetAddress) {
            /* Check if Block lies where the Kernel is Loaded */
            if (
                MMapEntry->Type == MBootDef::MemoryMapEntryType::AVAILABLE) {
                PhysicalFreeBlocks++;
                PhysicalMemoryMapUnset(BitmapIndex);
            } else {
                /* Block Overlaps Loaded Kernel Binary */
                PhysicalMemoryMapSet(BitmapIndex);
            }

            /* Update Base Address and Bitmap Index */
            BaseAddress += KERNEL_BOOTMEM_PMMGR_BLOCKSIZE;
            BitmapIndex++;
        }

        /* Prevent Alloc at 0x00 */
        PhysicalMemoryMapSet(0);

        /* Update Prev Bitmap Index */
        PrevBitmapIndex = BitmapIndex;
    }

    /* Memory Containing Kernel and Bitmap should be Marked as Used */
    u64 BaseAddress = ((u64)&KRNL_START);
    u64 OffsetAddress = AlignAddressToPage(((u64)&KRNL_END) + (PrevBitmapIndex / 64));
    u64 BitmapIndex = (BaseAddress / KERNEL_BOOTMEM_PMMGR_BLOCKSIZE);

    while (BaseAddress <= OffsetAddress) {
        PhysicalMemoryMapSet(BitmapIndex++);
        PhysicalFreeBlocks--;
        BaseAddress += KERNEL_BOOTMEM_PMMGR_BLOCKSIZE;
    }
}

/// @brief Sets up environment for Virtual Memory Manager to take over
void BootMem::InitVirtualMemory(MBootDef::MemoryMap* MemoryMap)
{
    /*
        This routine maps all the Physical Memory that's installed
        starting from the address 0xffff888000000000 in the Virtual
        Address Space. The page tables are dynamically allocated by
        obtained addresses from the physical memory manager.

        => This leads to a chicken-and-egg problem:
        Though we could obtain any number of Physical Memory Addresses
        from the pool, only the addresses in the first 2MB of memory
        are identity mapped to the virtual address space by OSLoader.

        To fix this, we monitor the virtual address space to ensure
        that we never exceed KERNEL_BOOTMEM_VMMGR_IDEXTENDTHRESHOLD.
        If we exceed threshold, we use the next available physical
        memory addresses to create a new page table that extends the
        identity map.

        Refer:
        https://wiki.osdev.org/Paging
    */

    for (
        MBootDef::MemoryMapEntry* MMapEntry = (MBootDef::MemoryMapEntry*)(MemoryMap + 1);
        ((u8*)MMapEntry) - ((u8*)(MemoryMap + 1)) < (MemoryMap->Header.Size - sizeof(MBootDef::MemoryMap));
        MMapEntry = (MBootDef::MemoryMapEntry*)((u8*)MMapEntry + MemoryMap->EntrySize)) {

        /* Get Memory Region Information */
        u64 BaseAddress = AlignAddressToPage(MMapEntry->BaseAddress);
        u64 OffsetAddress = AlignAddressToPage(MMapEntry->BaseAddress + MMapEntry->Length);

        int logl = 5;
        while (BaseAddress <= OffsetAddress) {
            VirtualAddress VirtBaseAddress = BaseAddress + KERNEL_VIRTMM_PHYMEM_MAPOFFSET;
            u64 PML4Index = VirtualMemory::GetPML4Index(VirtBaseAddress);
            u64 PDPTIndex = VirtualMemory::GetPDPTIndex(VirtBaseAddress);
            u64 PDTIndex = VirtualMemory::GetPDTIndex(VirtBaseAddress);
            u64 PTIndex = VirtualMemory::GetPTIndex(VirtBaseAddress);

            // lazy code. optimize on mvp.
            if (!osloader_pml4t.Entries[PML4Index]) {
                PhysicalAddress* AllocatedPDPT = PhysicalMemoryAllocateIDMappedBlock();
                osloader_pml4t.Entries[PML4Index] = (VirtualMemory::PML4Entry)AllocatedPDPT | 0;
                printf("\n=> PDPT CREATED: 0x"); // debug
                printf((u64)AllocatedPDPT, 16);
            }

            VirtualMemory::PDPTable* PDPTable = (VirtualMemory::PDPTable*)VirtualMemory::GetBaseAddress(osloader_pml4t.Entries[PML4Index]);
            if (!PDPTable->Entries[PDPTIndex]) {
                PhysicalAddress* AllocatedPDT = PhysicalMemoryAllocateIDMappedBlock();
                PDPTable->Entries[PDPTIndex] = (VirtualMemory::PDPEntry)AllocatedPDT | 0;
                printf("\n=> PDT CREATED: 0x");
                printf((u64)AllocatedPDT, 16);
            }

            VirtualMemory::PDTable* PDTable = (VirtualMemory::PDTable*)VirtualMemory::GetBaseAddress(PDPTable->Entries[PDPTIndex]);
            if (!PDTable->Entries[PDTIndex]) {
                PhysicalAddress* AllocatedPT = PhysicalMemoryAllocateIDMappedBlock();
                PDTable->Entries[PDTIndex] = (VirtualMemory::PDEntry)AllocatedPT | 0;
                // printf("\n=> Page Table CREATED: 0x");
                // printf((u64) AllocatedPT, 16);
            }

            /* Increment Base Address */
            BaseAddress += KERNEL_VIRTMM_PAGESIZE;
        }
    }
}