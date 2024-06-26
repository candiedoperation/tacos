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

#ifndef TOOLS_REPLIB_STRINGS_HPP
#define TOOLS_REPLIB_STRINGS_HPP

#include <kernel/types.hpp>

using namespace tacOS::Kernel;

namespace tacOS {
namespace Tools {
    namespace KernelRTL {
        i32 strncmp(const char* s1, const char* s2, usize len);
        void* memset(void* ptr, u8 value, u64 num);
    }
}
}

#endif