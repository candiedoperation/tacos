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

#ifndef DRIVERS_VIDEO_VGA_HPP
#define DRIVERS_VIDEO_VGA_HPP

#include <kernel/types.hpp>

using namespace tacos::Kernel;

namespace tacos {
namespace Drivers {
    namespace Video {
        enum vga_color {
            BLACK = 0x0,
            BLUE = 0x1,
            GREEN = 0x2,
            CYAN = 0x3,
            RED = 0x4,
            MAGENTA = 0x5,
            BROWN = 0x6,
            LIGHT_GRAY = 0x7,
            DARK_GRAY = 0x8,
            LIGHT_BLUE = 0x9,
            LIGHT_GREEN = 0xa,
            LIGHT_CYAN = 0xb,
            LIGHT_RED = 0xc,
            PINK = 0xd,
            YELLOW = 0xe,
            WHITE = 0xf
        };

        class VgaTextMode {
        private:
            static u8 ScreenWidth;
            static u8 ScreenHeight;
            static u8* MemoryAddress;
            static u16 CursorPos;

        public:
            static void BufferWrite(char* buffer);
            static void BufferWrite(char* buffer, vga_color FgColor, vga_color BgColor);
        };
    }
}
}

#endif