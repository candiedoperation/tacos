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

#include <kernel/types.hpp>
#include <tools/kernelrtl/strings.hpp>

using namespace tacOS::Tools;

/// @brief Standard Library Replacement for strncmp
i32 KernelRTL::strncmp(const char* s1, const char* s2, usize len)
{
    u8 cmp;
    i32 diff = 0;

    while (len-- && !(diff = *s1++ - (cmp = *s2++)) && cmp);
    return diff;
}

void* KernelRTL::memset(void* dest, u8 value, u64 len) {
    u8* ptr = (u8*) dest;
    while (len-- > 0) {
        *(ptr++) = value;
    }

    return dest;
}