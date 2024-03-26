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

#ifndef DRIVERS_HAL_VIRTKBD_HPP
#define DRIVERS_HAL_VIRTKBD_HPP

namespace tacos {
namespace Drivers {
    namespace HAL {
        class VirtualKbd {
        public:
            /// @brief tacOS Kernel Keycodes.
            enum VKey {
                /*
                    This enumeration consists of kernel keycodes for specific
                    keys. Inputs from PS/2, USB and software-based keyboards
                    are translated to VKey keycodes to support hardware inde
                    -pendent keyboard inputs. Keycodes are modeled after the
                    Linux Kernel.

                    Refer:
                    https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
                    https://manpages.ubuntu.com/manpages/jammy/man7/virkeycode-linux.7.html
                */

                RESERVED = 0,
                ESC = 1,
                VK_1 = 2,
                VK_2 = 3,
                VK_3 = 4,
                VK_4 = 5,
                VK_5 = 6,
                VK_6 = 7,
                VK_7 = 8,
                VK_8 = 9,
                VK_9 = 10,
                VK_0 = 11,

                MINUS = 12,
                EQUAL = 13,
                BACKSPACE = 14,
                TAB = 15,

                VK_Q = 16,
                VK_W = 17,
                VK_E = 18,
                VK_R = 19,
                VK_T = 20,
                VK_Y = 21,
                VK_U = 22,
                VK_I = 23,
                VK_O = 24,
                VK_P = 25,

                LEFTBRACE = 26,
                RIGHTBRACE = 27,
                ENTER = 28,
                LEFTCTRL = 29,

                VK_A = 30,
                VK_S = 31,
                VK_D = 32,
                VK_F = 33,
                VK_G = 34,
                VK_H = 35,
                VK_J = 36,
                VK_K = 37,
                VK_L = 38,

                SEMICOLON = 39,
                APOSTROPHE = 40,
                GRAVE = 41,
                LEFTSHIFT = 42,
                BACKSLASH = 43,

                VK_Z = 44,
                VK_X = 45,
                VK_C = 46,
                VK_V = 47,
                VK_B = 48,
                VK_N = 49,
                VK_M = 50,

                COMMA = 51,
                DOT = 52,
                SLASH = 53,
                RIGHTSHIFT = 54,
                KPASTERISK = 55, /* Keypad Asterisk */
                LEFTALT = 56,
                SPACE = 57,
                CAPSLOCK = 58,

                F1 = 59,
                F2 = 60,
                F3 = 61,
                F4 = 62,
                F5 = 63,
                F6 = 64,
                F7 = 65,
                F8 = 66,
                F9 = 67,
                F10 = 68,
                NUMLOCK = 69,
                SCRLOCK = 70,

                /* Keypad Keys */
                KP7 = 71,
                KP8 = 72,
                KP9 = 73,
                KPMINUS = 74,
                KP4 = 75,
                KP5 = 76,
                KP6 = 77,
                KPPLUS = 78,
                KP1 = 79,
                KP2 = 80,
                KP3 = 81,
                KP0 = 82,
                KPDOT = 83,

                /* Japanese Keys Omitted. Fix in Future. */
                F11 = 87,
                F12 = 88,
            };
        };
    }
}
}

#endif