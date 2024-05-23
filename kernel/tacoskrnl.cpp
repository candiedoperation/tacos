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

#include <kernel/mem/pagemap.hpp>
#include <tools/replib/replib.hpp>
#include <drivers/acpi/acpipvdr.hpp>
#include <kernel/mem/physicalmm.hpp>
#include <kernel/multiboot/mbpvdr.hpp>
#include <kernel/interrupts/intrdef.hpp>

using namespace tacOS::Tools::RepLib;
using namespace tacOS::Drivers::Acpi;
using namespace tacOS::Kernel;

void clear_screen()
{
    unsigned short* vm = (unsigned short*)0xb8000;
    for (int i = 0; i < 2000; i++) {
        vm[i] = ((unsigned short)0x0f << 8) | 0x00;
    }
}

extern "C" void LoadKernel(u64 MultibootInfoAddr)
{
    /* FUTURE: Take this off */
    clear_screen();

    /* Perform Early Initialization */
    Interrupt::Register();                         // FUTURE: IMPROVE ROUTINES, NAMING.
    MBootProvider::Initialize(MultibootInfoAddr);  // FUTURE: Returns Status, Use it
    PhysicalMemory::Initialize();

    /* FUTURE: Setup Linear Framebuffer Display */
    printf("\ntacOS Kernel Initializing...\n");
    
    /* Setup Memory */
    VirtMem::PageMap::Intialize();

    /* Setup ACPI */
    AcpiDef::Status AcpiInitStatus = AcpiProvider::Initialize();
    if (!AcpiInitStatus) {
        printf("ACPI Version Unsupported");
        __asm__ volatile("cli; hlt");
    }

    /* Enable HW Interrupts */
    //Interrupt::InitHWInterrupts();

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