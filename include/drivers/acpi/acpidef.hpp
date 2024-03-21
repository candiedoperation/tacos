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

#ifndef DRIVERS_ACPI_HPP
#define DRIVERS_ACPI_HPP

#include <kernel/types.hpp>

using namespace tacos::Kernel;

#define ACPI_EBDA_SEG 0x40E
#define ACPI_BIOS_MEM_STA 0x000E0000
#define ACPI_BIOS_MEM_END 0x000FFFFF
#define ACPI_RSDP_STLEN 20 /* Length of the RSDP Struct */
#define ACPI_XSDP_STLEN 36 /* Length of the XSDP Struct */

#define ACPI_SIG_RSDP "RSD PTR "
#define ACPI_SIG_FADT "FACP"
#define ACPI_SIG_MADT "APIC"

namespace tacos {
namespace Drivers {
    namespace Acpi {
        class AcpiDef {
        public:
            /// @brief XSDP Table Address
            typedef u8* RSDPAddress;
            typedef u32 Status;
            typedef u64 Address;

            /// @brief ACPI Versions and their Revision Code
            enum Version {
                INVALID = 0,
                ONE = 1,
                TWO = 2
            };

            /// @brief Common Header for all ACPI System Descriptor Tables
            struct SdtHeader {
                char Signature[4];
                u32 Length;
                u8 Revision;
                u8 Checksum;
                char OEMID[6];
                char OEMTableID[8];
                u32 OEMRevision;
                u32 CreatorID;
                u32 CreatorRevision;
            } __attribute__((packed));

            /// @brief The Root System Descriptor Pointer Structure
            struct Rsdp {
                char Signature[8];
                u8 Checksum;
                char OEMID[6];
                u8 Revision;
                u32 RsdtAddress;

                u32 Length;
                u64 XsdtAddress;
                u8 ExtendedChecksum;
                u8 Reserved[3];
            } __attribute__((packed));

            /// @brief Extended System Descriptor Table Structure
            struct Xsdt {
                /*
                    The XSDT is the main System Description Table. However there are 
                    many kinds of SDT. All the SDT may be split into two parts. One 
                    (the header) which is common to all the SDT an another (data) which 
                    is different for each table.

                    The SdtList consists of pointers to all available SDTs. The size is
                    set to one as it's a hack (struct hack) that lets initialize the 
                    array and access elements at any offset. In case of invalid access, 
                    it leads to undefined behavior.

                    Refer:
                    https://wiki.osdev.org/XSDT#Structure
                */

                struct SdtHeader Header;
                u64 SdtList[1] __attribute__((aligned(4)));
            }  __attribute__((packed));

            /// @brief Root System Description Table Structure
            struct Rsdt {
                /* Refer: https://wiki.osdev.org/RSDT */
                struct SdtHeader Header;
                u32 SdtList[1];
            };

            static RSDPAddress GetRSDPAddr();
            static Version GetACPIVersion(const Rsdp* XsdpTbl);
            static Status GetTableBySignature(char* Signature, Xsdt* Xsdt, Address* Table);
        };
    }
}
}

#endif