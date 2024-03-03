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

#include <asm/io.hpp>
#include <drivers/hal/pic8259.hpp>

using namespace tacos::Drivers::HAL;
using namespace tacos::ASM;

void Pic8259::remap()
{
}

/// @brief Intializes the 8259 Programmable Interrupt Contoller
void Pic8259::initialize()
{
    /*
        To Initialize the PIC, send the Intialization Code.
        Both the master and slave PICs are traditionally
        initialized together.

        After a Command is sent to the PIC, write gibberish to
        port 0x80 (Linux does it this way). Why? The PIC takes
        time to process the request. We need a small timer but,
        timers work on interrupts and we're writing this block
        of code to enable interrupts. The hack involves writing
        gibbersh that takes around 1 to 4 microseconds. It's
        guaranteed that the PIC will process the request by then.

        Refer:
        https://docs.rs/crate/pic8259/0.10.1/source/src/lib.rs
        https://wiki.osdev.org/8259_PIC#Initialisation
    */

    /* The first step is saving the current masks set in the PIC */
    u8 mask01 = IO::inb(PIC8259_MASTER_DATA);
    u8 mask02 = IO::inb(PIC8259_SLAVE_DATA);

    /* Intialize the Master PIC, Wait (for old motherboards) */
    IO::outb(PIC8259_MASTER, PIC8259_INIT);
    IO::wait();

    /* Intialize the Slave PIC, Wait (for old motherboards) */
    IO::outb(PIC8259_SLAVE, PIC8259_INIT);
    IO::wait();

    /* 
        Both PICs would wait for the next three
        Initialization Command Words (ICWs).

        Refer:
        https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf
    */
}