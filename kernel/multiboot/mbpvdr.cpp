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

#include <kernel/multiboot/mbpvdr.hpp>
#include <tools/kernelrtl/kernelrtl.hpp>

using namespace tacOS::Tools::KernelRTL;

/* Define Statics */
MBootDef::MultibootInfo* MBootProvider::MBootInfoPtr;
MBootDef::MemoryMap* MBootProvider::MemoryMapPtr;
MBootDef::MemoryInfo* MBootProvider::MemoryInfoPtr;

/// @brief Parses Multiboot Structres and Inits appropriate structures
/// @param MultibootInfoPtrAddress Multiboot2 Pointer Address from Bootloader
/// @return Initialization Status. (0 for Error)
int MBootProvider::Initialize(u64 MultibootInfoPtrAddress)
{
    /*
        This routine parses the Multiboot2 Structure received from
        the bootloader and calls appropriate sub-initialization rou
        -tines based on the various entries.

        Refer:
        https://forum.osdev.org/viewtopic.php?f=1&t=56304
        https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
    */

    printf("Reading Multiboot Configuration...\n");
    MBootInfoPtr = (MBootDef::MultibootInfo*)MultibootInfoPtrAddress;

    for (
        MBootDef::TagHeader* MBootInfoTag = MBootInfoPtr->Tags;
        MBootInfoTag->TagType != 0;
        MBootInfoTag = (MBootDef::TagHeader*)((u8*)MBootInfoTag + ((MBootInfoTag->Size + 7) & ~7))) {

        switch (MBootInfoTag->TagType) {
        case MBootDef::Tag::MEMORY_INFO: {
            ProcessMemoryInfo((MBootDef::MemoryInfo*)MBootInfoTag);
            break;
        }

        case MBootDef::Tag::MEMORY_MAP: {
            MBootProvider::MemoryMapPtr = (MBootDef::MemoryMap*)MBootInfoTag;
            break;
        }

        default: {
            // printf("MBoot Tag Type ");
            // printf(Tag->TagType);
            // printf(" -> Size: ");
            // printf(Tag->Size);
            // printf("\n");
            break;
        }
        }
    }

    /* FUTURE: Return Intialization Status */
    return 1;
}

/// @brief Processed obtained Memory Info Entry
/// @param MemoryInfo Pointer to Multiboot2 Memory Info Entry
void MBootProvider::ProcessMemoryInfo(MBootDef::MemoryInfo* MemoryInfo)
{
    printf("\nMemory Info (Legacy): ");
    printf("\n    Lower Memory: ");
    printf(MemoryInfo->MemLower);
    printf("KB");
    printf("\n    Upper Memory: ");
    printf(MemoryInfo->MemUpper);
    printf("KB\n\n");
}