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

#include <drivers/hal/apic.hpp>
#include <drivers/acpi/acpipvdr.hpp>
#include <drivers/video/vga.hpp>

using namespace tacos::Drivers::HAL;
using namespace tacos::Drivers::Acpi;
using namespace tacos::Drivers::Video; // take this off!


/// @brief Initializes the Advanced Programmable Interrupt Controller (APIC)
Apic::Status Apic::Initialize()
{
    /* We're assuming ACPI is Initialized by LoadKernel() */
    AcpiDef::Address MadtAddr;
    AcpiDef::GetTableBySignature(ACPI_SIG_MADT, AcpiProvider::Xsdt, &MadtAddr);
    AcpiDef::Madt* Madt = (AcpiDef::Madt*) MadtAddr;
    VgaTextMode::BufferWrite(Madt->Header.Signature);
    return Status::OK;
}