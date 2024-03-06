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

#include <drivers/acpi/acpitbl.hpp>
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

    VgaTextMode::BufferWrite("Hello World! This Function works Properly!\n", vga_color::WHITE, vga_color::GREEN);

    AcpiTable::GetRSDPAddr();
    Interrupt::Register();

    /* Check if Interrupts Work! */
    // int DivByZ = 1 / 0;
    //  int DivByZ2 = 1/0;

    for (;;) {
        /*
            Halt CPU till next Interrupt. This Prevents 100% 
            CPU Utilization and improves efficiency.
        */

        __asm__ volatile("hlt");
    }
}