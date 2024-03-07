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

#define ACPI_RSDP_STR "RSD PTR "
#define ACPI_EBDA_SEG 0x40E
#define ACPI_BIOS_MEM_STA 0x000E0000
#define ACPI_BIOS_MEM_END 0x000FFFFF

namespace tacos {
namespace Drivers {
    namespace Acpi {
        class AcpiDef {
        public:
            /// @brief XSDP Table Address
            typedef u8* RSDPAddress;

            /// @brief ACPI Versions and their Revision Code
            enum Version {
                INVALID = 0,
                ONE = 1,
                TWO = 2
            };

            struct XsdpTable {
                char Signature[8];
                u8 Checksum;
                char OEMID[6];
                u8 Revision;
                u32 RsdtAddress;

                u32 Length;
                u64 XsdtAddress;
                u8 ExtendedChecksum;
                u8 Reserved[3];
            };

            static RSDPAddress GetRSDPAddr();
            static Version GetACPIVersion(XsdpTable* XsdpTbl);
        };
    }
}
}

#endif