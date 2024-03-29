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

#include <drivers/acpi/acpipvdr.hpp>

using namespace tacos::Drivers::Acpi;

AcpiDef::RSDPAddress AcpiProvider::RsdpAddr;
AcpiDef::Rsdp* AcpiProvider::Rsdp;
AcpiDef::Xsdt* AcpiProvider::Xsdt;

/// @brief Intialize ACPI for Kernel
/// @return ACPI Initialization Status
AcpiDef::Status AcpiProvider::Initialize()
{
    /*
        This ACPI initialization routine tries to obtain the
        RSDP Address and access the RSDP structure stored in
        memory. Furthermore, it checks for ACPI Version 2.0+,
        fetches the Extended System Description Table (XSDT),
        saves them to static variables and completes the init
        process.

        Refer:
        https://wiki.osdev.org/RSDP
        https://wiki.osdev.org/XSDT
    */

    RsdpAddr = AcpiDef::GetRSDPAddr();
    if (RsdpAddr == 0)
        return 0;

    Rsdp = (AcpiDef::Rsdp*)RsdpAddr;
    if (AcpiDef::GetACPIVersion(Rsdp) != AcpiDef::Version::TWO) {
        /* Set Error Message as Param in future. */
        return 0;
    }

    Xsdt = (AcpiDef::Xsdt*)Rsdp->XsdtAddress;
    return 1;
}