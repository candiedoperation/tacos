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

#include <drivers/acpi/acpitbl.hpp>
#include <drivers/video/vga.hpp>

using namespace tacos::Drivers::Acpi;
using namespace tacos::Drivers::Video;

bool ValidateXsdpSignature(const char* Signature)
{
    bool SigValid = true;
    for (int i = 0; i < sizeof(Signature); i++) {
        if (Signature[i] != ACPI_RSDP_STR[i]) {
            SigValid = false;
            break;
        }
    }

    return SigValid;
}

/// @brief Tries to Find the RSDP Address using the BIOS Search Method
/// @return RSDP Address Location or 0 (if not found)
AcpiTable::RSDPAddress GetRSDPAddrBIOS()
{
    /*
       In Non-UEFI Modes:
       The RSDP is either located within the first 1 KB of the EBDA (Extended BIOS Data Area)
       (a 2 byte real mode segment pointer to it is located at 0x40E), or in the memory region
       from 0x000E0000 to 0x000FFFFF (the main BIOS area below 1 MB). To find the table, the
       Operating System has to find the "RSD PTR " string in one of the two areas. This
       signature is always on a 16 byte boundary.

       Refer:
       https://wiki.osdev.org/RSDP
       https://github.com/torvalds/linux/blob/master/include/acpi/actbl.h
       https://github.com/torvalds/linux/blob/master/arch/x86/boot/compressed/acpi.c
   */

    /* Probe Memory Regions 0x000E0000 to 0x000FFFFF */
    bool FoundSignature = false;
    for (int MemLoc = ACPI_BIOS_MEM_STA; MemLoc <= ACPI_BIOS_MEM_END; MemLoc += 16) {
        const u8* MemBlock = (u8*) MemLoc;
        const AcpiTable::XsdpTable* XsdpTable = (AcpiTable::XsdpTable*)MemBlock;
        FoundSignature = ValidateXsdpSignature(XsdpTable->Signature);
        if (FoundSignature == true) return (AcpiTable::RSDPAddress) MemLoc;
    }

    /* Get Pointer to EBDA and Probe first 1KB */

    return 0;
}

/// @brief Tries to Find the RSDP Address using the UEFI Table
/// @return RSDP Address Location or 0 (if not found)
AcpiTable::RSDPAddress GetRSDPAddrUEFI()
{
    return 0;
}

/// @brief Gets the ACPI Root System Description Pointer (RDSP)
/// @return Pointer to RSD or 0 (if not found)
AcpiTable::RSDPAddress AcpiTable::GetRSDPAddr()
{
    AcpiTable::RSDPAddress RsdpAddr = GetRSDPAddrBIOS();
    VgaTextMode::BufferWrite((char *) ((RsdpAddr != 0) ? "RSDP Found" : "RSDP Not Found"));

    return 0;
}