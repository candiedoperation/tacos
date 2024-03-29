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

#include <drivers/acpi/acpidef.hpp>
#include <drivers/video/vga.hpp>
#include <tools/replib/replib.hpp>

using namespace tacos::Drivers::Acpi;
using namespace tacos::Drivers::Video;
using namespace tacos::Tools::RepLib;

/// @brief Checks if current string is RSD PTR
/// @param Signature Pointer to 8 byte Signature String
/// @return True or False based on Validity
bool ValidateXsdpSignature(const char* Signature)
{
    return !(strncmp(
        Signature,
        ACPI_SIG_RSDP,
        sizeof(Signature)));
}

/// @brief Validates the RSDP or XSDP Checksum
/// @param Rsdp Pointer to the XSDP Table
/// @return True or False based on Validity
bool ValidateXsdpChecksum(const AcpiDef::Rsdp* Rsdp)
{
    /*
        Before the RSDP is relied upon you should check that the checksum
        is valid. For ACPI 1.0 (the first structure) you add up every byte
        in the structure and make sure the lowest byte of the result is equal
        to zero. For ACPI 2.0 and later you'd do exactly the same thing for
        the original (ACPI 1.0) part of the second structure, and then do it
        again for the fields that are part of the ACPI 2.0 extension.

        Refer:
        https://wiki.osdev.org/RSDP#Checksum_validation
    */

    bool XsdpChecksumValid = false;

    u32 RsdpByteSum = 0;
    u8* XsdpPtr = (u8*)Rsdp;

    /* Add all bytes in structure */
    for (u8 Offset = 0; Offset <= ACPI_RSDP_STLEN; Offset++) {
        RsdpByteSum += XsdpPtr[Offset];
    }

    /* Lowest Byte of RsdpByteSum is Zero for Valid Structures */
    XsdpChecksumValid = ((RsdpByteSum & 0xFF) == 0);

    if (AcpiDef::GetACPIVersion(Rsdp) >= AcpiDef::Version::TWO) {
        /* FUTURE: merge this into a single fn like linux does */
        u32 XsdpByteSum = 0;
        for (u8 Offset = 0; Offset <= ACPI_XSDP_STLEN; Offset++) {
            XsdpByteSum += XsdpPtr[Offset];
        }

        XsdpChecksumValid = ((XsdpByteSum & 0xFF) == 0);
    }

    return XsdpChecksumValid;
}

/// @brief 
/// @param TableHeader 
/// @return 
bool ValidateSDTChecksum(const AcpiDef::SdtHeader* TableHeader) {
    
}

/// @brief Tries to Find the RSDP Address using the BIOS Search Method
/// @return RSDP Address Location or 0 (if not found)
AcpiDef::RSDPAddress GetRSDPAddrBIOS()
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
        const u8* MemBlock = (u8*)MemLoc;
        const AcpiDef::Rsdp* Rsdp = (AcpiDef::Rsdp*)MemBlock;
        if (ValidateXsdpSignature(Rsdp->Signature) && ValidateXsdpChecksum(Rsdp))
            return (AcpiDef::RSDPAddress)MemLoc;
    }

    /* Get Pointer to EBDA and Probe first 1KB */

    return 0;
}

/// @brief Tries to Find the RSDP Address using the UEFI Table
/// @return RSDP Address Location or 0 (if not found)
AcpiDef::RSDPAddress GetRSDPAddrUEFI()
{
    return 0;
}

/// @brief Gets the ACPI Root System Description Pointer (RDSP)
/// @return Pointer to RSD or 0 (if not found)
AcpiDef::RSDPAddress AcpiDef::GetRSDPAddr()
{
    /* FIX LATER: USE UEFI GET FUNC IF IN UEFI */
    return GetRSDPAddrBIOS();
}

/// @brief Parses the ACPI Version from RSDP or XSDP
/// @param Rsdp Pointer to RSDP or XSDP
/// @return ACPI Version Enumerated Type
AcpiDef::Version AcpiDef::GetACPIVersion(const AcpiDef::Rsdp* Rsdp)
{
    /*
        The ACPI Version can be detected using the Revision field in the RSDP.
        If this field contains 0, then ACPI Version 1.0 is used. For subsequent
        versions (ACPI version 2.0 to 6.1), the value 2 is used. The exact version
        of ACPI can be deduced via the FADT table.

        Refer:
        https://wiki.osdev.org/RSDP#Detecting_ACPI_Version
    */

    u8 AcpiRevision = Rsdp->Revision;
    if (AcpiRevision == 0)
        return AcpiDef::Version::ONE;
    else if (AcpiRevision == 2) {
        /* FIX LATER: ASSUMING V2.0, PARSE FADT TABLE */
        return AcpiDef::Version::TWO;
    }

    return AcpiDef::Version::INVALID;
}

/// @brief Fetch ACPI Table using its Signature
/// @param Signature ACPI Table Signature
/// @param Xsdt Pointer to Root SDT (XSDT)
/// @param Table [out] Pointer to Output Table Address
/// @return ACPI Status
AcpiDef::Status AcpiDef::GetTableBySignature(
    char* Signature,
    AcpiDef::Xsdt* Xsdt,
    AcpiDef::Address* Table)
{
    /*
        The XSDT Length parameter indicates the total size of the table,
        inclusive of the header.

        Find the number of SdtList entries in the Root SDT by subtracting
        the total length field of the Root SDT from its header size. The
        resulting value is the number of bytes. XSDT values are 8 bytes
        in length. Divide the resulting value by 8 to get the entries.
    */

    u32 SdtEntries = ((Xsdt->Header.Length - sizeof(Xsdt->Header)) / 8);
    for (u32 Offset = 0; Offset < SdtEntries; Offset++) {
        AcpiDef::SdtHeader* TableHeader = (AcpiDef::SdtHeader*)Xsdt->SdtList[Offset];
        if (!strncmp(TableHeader->Signature, Signature, 4)) {
            *Table = Xsdt->SdtList[Offset]; /* Set Ptr to Mem Addr. */
            return (AcpiDef::Status) 1; /* FIX IN FUTURE. */
        }
    }

    /* Table Not Found */
    return 0;
}