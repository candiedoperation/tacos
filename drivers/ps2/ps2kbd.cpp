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

#include <drivers/ps2/ps2kbd.hpp>

using namespace tacos::Drivers::HAL;

/// @brief PS/2 Keyboard Scan Code Set 01
static const VirtualKbd::VKey ScanCodeS1[512] = {
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::ESC,
    VirtualKbd::VKey::VK_1,
    VirtualKbd::VKey::VK_2,
    VirtualKbd::VKey::VK_3,
    VirtualKbd::VKey::VK_4,
    VirtualKbd::VKey::VK_5,
    VirtualKbd::VKey::VK_6,
    VirtualKbd::VKey::VK_7,
    VirtualKbd::VKey::VK_8,
    VirtualKbd::VKey::VK_9,
    VirtualKbd::VKey::VK_0,

    VirtualKbd::VKey::MINUS,
    VirtualKbd::VKey::EQUAL,
    VirtualKbd::VKey::BACKSPACE,
    VirtualKbd::VKey::TAB,
    VirtualKbd::VKey::VK_Q,
    VirtualKbd::VKey::VK_W,
    VirtualKbd::VKey::VK_E,
    VirtualKbd::VKey::VK_R,
    VirtualKbd::VKey::VK_T,
    VirtualKbd::VKey::VK_Y,
    VirtualKbd::VKey::VK_U,
    VirtualKbd::VKey::VK_I,
    VirtualKbd::VKey::VK_O,
    VirtualKbd::VKey::VK_P,
    VirtualKbd::VKey::LEFTBRACE,
    VirtualKbd::VKey::RIGHTBRACE,

    VirtualKbd::VKey::ENTER,
    VirtualKbd::VKey::LEFTCTRL,
    VirtualKbd::VKey::VK_A,
    VirtualKbd::VKey::VK_S,
    VirtualKbd::VKey::VK_D,
    VirtualKbd::VKey::VK_F,
    VirtualKbd::VKey::VK_G,
    VirtualKbd::VKey::VK_H,
    VirtualKbd::VKey::VK_J,
    VirtualKbd::VKey::VK_K,
    VirtualKbd::VKey::VK_L,
    VirtualKbd::VKey::SEMICOLON,
    VirtualKbd::VKey::APOSTROPHE,
    VirtualKbd::VKey::GRAVE,
    VirtualKbd::VKey::LEFTSHIFT,
    VirtualKbd::VKey::BACKSLASH,

    VirtualKbd::VKey::VK_Z,
    VirtualKbd::VKey::VK_X,
    VirtualKbd::VKey::VK_C,
    VirtualKbd::VKey::VK_V,
    VirtualKbd::VKey::VK_B,
    VirtualKbd::VKey::VK_N,
    VirtualKbd::VKey::VK_M,
    VirtualKbd::VKey::COMMA,
    VirtualKbd::VKey::DOT,
    VirtualKbd::VKey::SLASH,
    VirtualKbd::VKey::RIGHTSHIFT,
    VirtualKbd::VKey::KPASTERISK,
    VirtualKbd::VKey::LEFTALT,
    VirtualKbd::VKey::SPACE,
    VirtualKbd::VKey::CAPSLOCK,

    VirtualKbd::VKey::F1,
    VirtualKbd::VKey::F2,
    VirtualKbd::VKey::F3,
    VirtualKbd::VKey::F4,
    VirtualKbd::VKey::F5,
    VirtualKbd::VKey::F6,
    VirtualKbd::VKey::F7,
    VirtualKbd::VKey::F8,
    VirtualKbd::VKey::F9,
    VirtualKbd::VKey::F10,
    VirtualKbd::VKey::NUMLOCK,
    VirtualKbd::VKey::SCRLOCK,

    VirtualKbd::VKey::KP7,
    VirtualKbd::VKey::KP8,
    VirtualKbd::VKey::KP9,
    VirtualKbd::VKey::KPMINUS,
    VirtualKbd::VKey::KP4,
    VirtualKbd::VKey::KP5,
    VirtualKbd::VKey::KP6,
    VirtualKbd::VKey::KPPLUS,
    VirtualKbd::VKey::KP1,
    VirtualKbd::VKey::KP2,
    VirtualKbd::VKey::KP3,
    VirtualKbd::VKey::KP0,
    VirtualKbd::VKey::KPDOT,

    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::F11,
    VirtualKbd::VKey::F12,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,

    VirtualKbd::VKey::RESERVED, /* 0x81 Release Combination */
    VirtualKbd::VKey::ESC,
    VirtualKbd::VKey::VK_1,
    VirtualKbd::VKey::VK_2,
    VirtualKbd::VKey::VK_3,
    VirtualKbd::VKey::VK_4,
    VirtualKbd::VKey::VK_5,
    VirtualKbd::VKey::VK_6,
    VirtualKbd::VKey::VK_7,
    VirtualKbd::VKey::VK_8,
    VirtualKbd::VKey::VK_9,
    VirtualKbd::VKey::VK_0,

    VirtualKbd::VKey::MINUS,
    VirtualKbd::VKey::EQUAL,
    VirtualKbd::VKey::BACKSPACE,
    VirtualKbd::VKey::TAB,
    VirtualKbd::VKey::VK_Q,
    VirtualKbd::VKey::VK_W,
    VirtualKbd::VKey::VK_E,
    VirtualKbd::VKey::VK_R,
    VirtualKbd::VKey::VK_T,
    VirtualKbd::VKey::VK_Y,
    VirtualKbd::VKey::VK_U,
    VirtualKbd::VKey::VK_I,
    VirtualKbd::VKey::VK_O,
    VirtualKbd::VKey::VK_P,
    VirtualKbd::VKey::LEFTBRACE,
    VirtualKbd::VKey::RIGHTBRACE,

    VirtualKbd::VKey::ENTER,
    VirtualKbd::VKey::LEFTCTRL,
    VirtualKbd::VKey::VK_A,
    VirtualKbd::VKey::VK_S,
    VirtualKbd::VKey::VK_D,
    VirtualKbd::VKey::VK_F,
    VirtualKbd::VKey::VK_G,
    VirtualKbd::VKey::VK_H,
    VirtualKbd::VKey::VK_J,
    VirtualKbd::VKey::VK_K,
    VirtualKbd::VKey::VK_L,
    VirtualKbd::VKey::SEMICOLON,
    VirtualKbd::VKey::APOSTROPHE,
    VirtualKbd::VKey::GRAVE,
    VirtualKbd::VKey::LEFTSHIFT,
    VirtualKbd::VKey::BACKSLASH,

    VirtualKbd::VKey::VK_Z,
    VirtualKbd::VKey::VK_X,
    VirtualKbd::VKey::VK_C,
    VirtualKbd::VKey::VK_V,
    VirtualKbd::VKey::VK_B,
    VirtualKbd::VKey::VK_N,
    VirtualKbd::VKey::VK_M,
    VirtualKbd::VKey::COMMA,
    VirtualKbd::VKey::DOT,
    VirtualKbd::VKey::SLASH,
    VirtualKbd::VKey::RIGHTSHIFT,
    VirtualKbd::VKey::KPASTERISK,
    VirtualKbd::VKey::LEFTALT,
    VirtualKbd::VKey::SPACE,
    VirtualKbd::VKey::CAPSLOCK,

    VirtualKbd::VKey::F1,
    VirtualKbd::VKey::F2,
    VirtualKbd::VKey::F3,
    VirtualKbd::VKey::F4,
    VirtualKbd::VKey::F5,
    VirtualKbd::VKey::F6,
    VirtualKbd::VKey::F7,
    VirtualKbd::VKey::F8,
    VirtualKbd::VKey::F9,
    VirtualKbd::VKey::F10,
    VirtualKbd::VKey::NUMLOCK,
    VirtualKbd::VKey::SCRLOCK,

    VirtualKbd::VKey::KP7,
    VirtualKbd::VKey::KP8,
    VirtualKbd::VKey::KP9,
    VirtualKbd::VKey::KPMINUS,
    VirtualKbd::VKey::KP4,
    VirtualKbd::VKey::KP5,
    VirtualKbd::VKey::KP6,
    VirtualKbd::VKey::KPPLUS,
    VirtualKbd::VKey::KP1,
    VirtualKbd::VKey::KP2,
    VirtualKbd::VKey::KP3,
    VirtualKbd::VKey::KP0,
    VirtualKbd::VKey::KPDOT,

    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::F11,
    VirtualKbd::VKey::F12,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
};

/// @brief PS/2 Keyboard Scan Code Set 02
static const VirtualKbd::VKey ScanCodeS2[512] = {
    /*
        Consists of Scan Code definitions from the second set
        of PS/2 scancodes mapped with tacOS Virtual Keys. The
        PS/2 Scan Code (Set 02) value for F9 is 0x01. This
        array maps such that ScanCodeS2[0x01] = VirtualKbd::VKey::F9.

        Refer:
        https://wiki.osdev.org/PS2_Keyboard#Scan_Code_Set_2
        http://lxr.linux.no/#linux+v3.5.4/drivers/input/keyboard/atkbd.c
    */

    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::F9,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::F5,
    VirtualKbd::VKey::F3,
    VirtualKbd::VKey::F1,
    VirtualKbd::VKey::F2,
    VirtualKbd::VKey::F12,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::F10,
    VirtualKbd::VKey::F8,
    VirtualKbd::VKey::F6,
    VirtualKbd::VKey::F4,
    VirtualKbd::VKey::TAB,
    VirtualKbd::VKey::GRAVE,
    VirtualKbd::VKey::RESERVED,

    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::LEFTALT,
    VirtualKbd::VKey::LEFTSHIFT,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::LEFTCTRL,
    VirtualKbd::VKey::VK_Q,
    VirtualKbd::VKey::VK_1,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::VK_Z,
    VirtualKbd::VKey::VK_S,
    VirtualKbd::VKey::VK_A,
    VirtualKbd::VKey::VK_W,
    VirtualKbd::VKey::VK_2,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,

    VirtualKbd::VKey::VK_C,
    VirtualKbd::VKey::VK_X,
    VirtualKbd::VKey::VK_D,
    VirtualKbd::VKey::VK_E,
    VirtualKbd::VKey::VK_4,
    VirtualKbd::VKey::VK_3,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::SPACE,
    VirtualKbd::VKey::VK_V,
    VirtualKbd::VKey::VK_F,
    VirtualKbd::VKey::VK_T,
    VirtualKbd::VKey::VK_R,
    VirtualKbd::VKey::VK_5,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::VK_N,
    VirtualKbd::VKey::VK_B,
    VirtualKbd::VKey::VK_H,

    VirtualKbd::VKey::VK_G,
    VirtualKbd::VKey::VK_Y,
    VirtualKbd::VKey::VK_6,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::VK_M,
    VirtualKbd::VKey::VK_J,
    VirtualKbd::VKey::VK_U,
    VirtualKbd::VKey::VK_7,
    VirtualKbd::VKey::VK_8,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::COMMA,
    VirtualKbd::VKey::VK_K,
    VirtualKbd::VKey::VK_I,
    VirtualKbd::VKey::VK_O,
    VirtualKbd::VKey::VK_0,
    VirtualKbd::VKey::VK_9,
    VirtualKbd::VKey::RESERVED,

    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::DOT,
    VirtualKbd::VKey::SLASH,
    VirtualKbd::VKey::VK_L,
    VirtualKbd::VKey::SEMICOLON,
    VirtualKbd::VKey::VK_P,
    VirtualKbd::VKey::MINUS,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::APOSTROPHE,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::LEFTBRACE,
    VirtualKbd::VKey::EQUAL,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::CAPSLOCK,
    VirtualKbd::VKey::RIGHTSHIFT,
    VirtualKbd::VKey::ENTER,
    VirtualKbd::VKey::RIGHTBRACE,

    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::BACKSLASH,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::BACKSPACE,
    VirtualKbd::VKey::RESERVED,

    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::KP1,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::KP4,
    VirtualKbd::VKey::KP7,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::KP0,
    VirtualKbd::VKey::KPDOT,
    VirtualKbd::VKey::KP2,
    VirtualKbd::VKey::KP5,
    VirtualKbd::VKey::KP6,
    VirtualKbd::VKey::KP8,
    VirtualKbd::VKey::ESC,
    VirtualKbd::VKey::NUMLOCK,

    VirtualKbd::VKey::F11,
    VirtualKbd::VKey::KPPLUS,
    VirtualKbd::VKey::KP3,
    VirtualKbd::VKey::KPMINUS,
    VirtualKbd::VKey::KPASTERISK,
    VirtualKbd::VKey::KP9,
    VirtualKbd::VKey::SCRLOCK,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::RESERVED,
    VirtualKbd::VKey::F7
};

/// @brief Handle PS/2 Keyboard Interrupts, Translate to VKey Inputs.
/// @param ScanCode Scan Code obtained from PS/2 Controller
void Ps2Kbd::KeyboardInterrupt(u8 ScanCode)
{
    /*
        This routine handles converting PS/2 Keyboard inputs,
        based on the current controller settings, into tacOS
        Virtual Keyboard inputs.

        Refer:
        https://wiki.osdev.org/PS2_Keyboard#Scan_Code_Sets.2C_Scan_Codes_and_Key_Codes
    */

    /* TODO: get code based on current key set */
    VirtualKbd::VKey Code = ScanCodeS1[ScanCode];

    if ((ScanCode & 128) != 128)
        VirtualKbd::KeyPressed(Code);
}