/*
    tacOS
    Copyright (C) 2023  Atheesh Thirumalairajan

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
#include <drivers/video/vga.hpp>
#include <kernel/interrupt.hpp>

using namespace tacos::Drivers::Video;
using namespace tacos::Drivers::Acpi;
using namespace tacos::Kernel;

void clear_screen()
{
    unsigned short* vm = (unsigned short*)0xb8000;
    for (int i = 0; i < 2000; i++) {
        vm[i] = ((unsigned short)0x0f << 8) | 0x00;
    }
}

extern "C" void LoadKernel()
{
    /* Kernel Entrypoint */
    clear_screen();

    VgaTextMode::BufferWrite("tacOS Kernel Initializing...\n", VgaTextMode::Color::YELLOW, VgaTextMode::Color::BLACK);

    /* Initialize ACPI */
    AcpiDef::Status AcpiInitStatus = AcpiProvider::Initialize();
    if (!AcpiInitStatus) {
        VgaTextMode::BufferWrite("ACPI Version Unsupported");
        __asm__ volatile("cli; hlt");
    }

    /* FUTURE: This Goes to Interrupt Register for IO/APIC Stuff. */
    AcpiDef::Address MadtAddr;
    AcpiDef::GetTableBySignature(ACPI_SIG_MADT, AcpiProvider::Xsdt, &MadtAddr);
    AcpiDef::Madt* Madt = (AcpiDef::Madt*) MadtAddr;
    VgaTextMode::BufferWrite(Madt->Header.Signature);

    Interrupt::Register();

    /* Check if CPU Exceptions and Interrupts Interrupts Work! */
    // int DivByZ = 1/0;

    for (;;) {
        /*
            Halt CPU till next Interrupt. This Prevents 100%
            CPU Utilization and improves efficiency.
        */

        __asm__ volatile("hlt");
    }
}