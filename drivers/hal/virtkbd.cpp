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

#include <drivers/hal/virtkbd.hpp>
#include <drivers/video/vga.hpp>

using namespace tacos::Drivers::HAL;
using namespace tacos::Drivers::Video;

u8 VirtualKbd::GetAsciiCode(VKey KeyCode) {
    return KeyCode + 65;
}

void VirtualKbd::KeyPressed(VKey KeyCode) {
    char a[2];
    a[0] = GetAsciiCode(KeyCode);
    a[1] = '\0';
    VgaTextMode::BufferWrite(a);
}