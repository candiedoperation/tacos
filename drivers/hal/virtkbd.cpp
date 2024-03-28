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

u8 VirtualKbd::GetAsciiCode(VKey KeyCode)
{
    /* FUTURE IMPL SHOULD INVOLVE CURRENT CHARSET, LANGUAGE */
    switch (KeyCode) {
    case VKey::VK_0:
        return 0x30;
    case VKey::VK_1:
        return 0x31;
    case VKey::VK_2:
        return 0x32;
    case VKey::VK_3:
        return 0x33;
    case VKey::VK_4:
        return 0x34;
    case VKey::VK_5:
        return 0x35;
    case VKey::VK_6:
        return 0x36;
    case VKey::VK_7:
        return 0x37;
    case VKey::VK_8:
        return 0x38;
    case VKey::VK_9:
        return 0x39;
    default:
        return 0;
    }
}

void VirtualKbd::KeyPressed(VKey KeyCode)
{
    char a[2];
    a[0] = GetAsciiCode(KeyCode);
    a[1] = '\0';
    VgaTextMode::BufferWrite(a);
}