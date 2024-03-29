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

#ifndef DRIVERS_HAL_PS2KBD_HPP
#define DRIVERS_HAL_PS2KBD_HPP

#include <drivers/hal/virtkbd.hpp>
#include <kernel/types.hpp>

using namespace tacos::Kernel;

namespace tacos {
namespace Drivers {
    namespace PS2 {
        class Keyboard {
        public:
            static void KeyboardInterrupt(u8 ScanCode);
        };
    }
}
}

#endif