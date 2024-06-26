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

#ifndef DRIVERS_HAL_PIC8259_HPP
#define DRIVERS_HAL_PIC8259_HPP

#include <kernel/types.hpp>
using namespace tacOS::Kernel;

#define PIC8259_INIT 0x11 /* PIC Initialization Instruction */
#define PIC8259_EOI 0x20 /* End of Interrupt Acknowledgement */
#define PIC8259_ICW1_ICW4 0x01 /* ICW1 message: ICW4 Will Be Present */
#define PIC8259_ICW4_8086 0x01 /* Set 8086 Mode */

/*
    Define PIC Controller Data and Input Ports
    Chip         Purpose 	I/O port
    Master PIC   Command 	0x0020
    Master PIC   Data 	    0x0021
    Slave PIC    Command 	0x00A0
    Slave PIC    Data 	    0x00A1

    Refer:
    https://wiki.osdev.org/8259_PIC
*/

#define PIC8259_MASTER 0x0020
#define PIC8259_MASTER_DATA 0x0021
#define PIC8259_MASTER_OFFSET 32

#define PIC8259_SLAVE 0x00A0
#define PIC8259_SLAVE_DATA 0x00A1
#define PIC8259_SLAVE_OFFSET (PIC8259_MASTER_OFFSET + 8)
#define PIC8259_MIN_IRQ PIC8259_MASTER_OFFSET
#define PIC8259_MAX_IRQ (PIC8259_SLAVE_OFFSET + 8)

namespace tacOS {
namespace Drivers {
    namespace HAL {
        class Pic8259 {
        public:
            /// @brief Standard ISA Interrupt Request Codes
            enum Irq {
                TIMER = 0,
                KEYBOARD = 1,
                CASCADE = 2,
                COM2 = 3,
                COM1 = 4,
                LPT2 = 5,
                FLOPPY_DISK = 6,
                LPT1 = 7, /* Usually the Spurious Interrupt. IRQ 7 */
                CMOS_RTC = 8,
                CUSTOM01 = 9,
                CUSTOM02 = 10,
                CUSTOM03 = 11,
                PS2_MOUSE = 12,
                FPU = 13,
                ATA_HDD_1 = 14,
                ATA_HDD_2 = 15
            };

            static void Initialize();
            static void Disable();
            static void EndOfInterrupt(u8 Code);
            static void TranslateInterrupt(u8 InterruptCode);
        };
    }
}
}

#endif