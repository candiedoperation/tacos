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

/* Initialize Static Variables */
bool VirtualKbd::CapsLockOn = false;

/// @brief Array that maps VKey Codes to ASCII Characters
static const u8 AsciiKeycodeMap[VIRTKBD_MAX_VKEY] = {
    0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x30, 0x2D, 0x3D, 0x08, 0x00, 0x51, 0x57,
    0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4F, 0x50, 0x5B,
    0x5D, 0x0A, 0x00, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48,
    0x4A, 0x4B, 0x4C, 0x3B, 0x27, 0x60, 0x00, 0x5C, 0x5A,
    0x58, 0x43, 0x56, 0x42, 0x4E, 0x4D, 0x2C, 0x2E, 0x2F,
    0x00, 0x2A, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37,
    0x38, 0x39, 0x2D, 0x34, 0x35, 0x36, 0x2B, 0x31, 0x32,
    0x33, 0x30, 0x2E
};

/// @brief Returns the Type of KeyCode
/// @param KeyCode tacOS Virtual Key Code
/// @return 0 for Miscellaneous, 1 for Alphabet
u8 VirtualKbd::GetVKeyType(VKey KeyCode)
{
    if ((VKey::VK_Q <= KeyCode && KeyCode <= VKey::VK_P)
        || (VKey::VK_A <= KeyCode && KeyCode <= VKey::VK_L)
        || (VKey::VK_Z <= KeyCode && KeyCode <= VKey::VK_M))
        return 1;
    else
        return 0;
}

void VirtualKbd::KeyPressed(VKey KeyCode)
{
    /* If Caps Lock Toggled? -> Add Offset for small chars */
    /* Future: Move ASCII Stuff to userspace from Kernel Layer */

    switch (KeyCode) {
    case VKey::CAPSLOCK: {
        CapsLockOn = !CapsLockOn;
        break;
    }

    default: {
        /* Get ASCII Code for current Virtual Key Code */
        u8 AsciiCode = AsciiKeycodeMap[KeyCode];

        /* For small letters, shift retreived ASCII code by 32 (A = 65, a = 97) */
        if (!CapsLockOn && (GetVKeyType(KeyCode) == 1))
            AsciiCode += 32;

        char* input = " ";
        input[0] = AsciiCode;
        VgaTextMode::BufferWrite(input);
        break;
    }
    }
}