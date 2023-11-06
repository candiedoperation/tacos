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

#include <drivers/video/vga.hpp>
#include <kernel/interrupt.hpp>

using namespace tacos::Drivers::Video;
using namespace tacos::Kernel;

void clear_screen() {
    unsigned short* vm = (unsigned short*) 0xb8000;
    for (int i = 0; i < 2000; i++) {
        vm[i] = ((unsigned short) 0x0f << 8) | 0x00;
    }
}

extern "C" void LoadKernel() {
    /* Kernel Entrypoint */
    clear_screen();

    VgaTextMode VgaTm;
    VgaTm.BufferWrite("Hello World! This Function works Properly!", VgaColor::WHITE, VgaColor::GREEN);

    Interrupt Intr;
    Intr.Register();

    /* Check if Interrupts Work! */
    int DivByZ = 1/0;

    while(1 == 1) {
        /* Prevent Exit */
    }
}