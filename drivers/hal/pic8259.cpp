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

/// @brief Intializes the 8259 Programmable Interrupt Contoller
void Pic8259::Initialize()
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
    IO::outb(PIC8259_MASTER, PIC8259_INIT | PIC8259_ICW1_ICW4);
    IO::wait();

    /* Intialize the Slave PIC with Init and ICW4 Present */
    IO::outb(PIC8259_SLAVE, PIC8259_INIT | PIC8259_ICW1_ICW4);
    IO::wait();

    /*
        Both PICs would wait for the next three
        Initialization Command Words (ICWs).

            1. Set ICW2: Offset Mapping for Interrupts
            2. Set ICW3: Inform master about slave
            3. Set ICW4: Make PIC use 8086 Mode

        Refer:
        https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf
    */

    /* Set Offset. Protected and Long modes reserve interrupts 0-31 for CPU */
    const u8 MASTER_OFFSET = 32;
    const u8 SLAVE_OFFSET = (MASTER_OFFSET + 8);
    IO::outb(PIC8259_MASTER_DATA, MASTER_OFFSET);
    IO::wait();
    IO::outb(PIC8259_SLAVE_DATA, SLAVE_OFFSET);
    IO::wait();

    /*
        Configure Cascading.
        1. Tell Master about Slave. Bits for Slave precense from 0 through 7.
        2. If slave ID is 3 (numbering starts 0), the 4th bit in Master ICW
           should be 1.

        Refer:
        Page 12, https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf
    */

    const u8 MASTER_ICW3 = 0b00001000; /* Slave at 3rd IRQ */
    const u8 SLAVE_ICW3 = 0b00000011; /* Slave ID is 3 */
    IO::outb(PIC8259_MASTER_DATA, MASTER_ICW3);
    IO::wait();
    IO::outb(PIC8259_SLAVE_DATA, SLAVE_ICW3);
    IO::wait();

    /* Make Master and Slave PIC use 8086 Mode (Using ICW4) */
    IO::outb(PIC8259_MASTER_DATA, PIC8259_ICW4_8086);
    IO::wait();
    IO::outb(PIC8259_SLAVE_DATA, PIC8259_ICW4_8086);
    IO::wait();

    /* Restore the Masks in Both PICs. Then, they're ready for interrupts */
    IO::outb(PIC8259_MASTER, mask01);
    IO::outb(PIC8259_SLAVE, mask02);
}

/// @brief Sends EOI Acknowledgement after every Interrupt.
void Pic8259::EndOfInterrupt(u8 Code)
{
    /*
        To Send the Next Interrupt, the PIC expects an
        End of Interrupt Message to acknowledge that the
        previous interrupt has been processed.

        If the IRQ is from the master, send EOI to Master Only.
        If the IRQ is from the slave, send it to both Master and Slave.
    */

    if (Code > 39)
        IO::outb(PIC8259_SLAVE, PIC8259_EOI);

    IO::outb(PIC8259_MASTER, PIC8259_EOI);
}