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
#include <tools/replib/replib.hpp>

using namespace tacOS::Tools::RepLib;

/* Define Statics */
MBootDef::MultibootInfo* MBootProvider::MBootInfoPtr;

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

    MBootInfoPtr = (MBootDef::MultibootInfo*)MultibootInfoPtrAddress;
    for (
        MBootDef::MultibootTag* Tag = MBootInfoPtr->Tags;
        Tag->TagType != 0;
        Tag = (MBootDef::MultibootTag*)((u8*)Tag + ((Tag->Size + 7) & ~7))
    ) {
        printf("MBoot Tag Type ");
        printf(Tag->TagType);
        printf(" -> Size: ");
        printf(Tag->Size);
        printf("\n");
    }

    /* FUTURE: Return Intialization Status */
    return 1;
}