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

#ifndef TOOLS_REPLIB_PRINTF_HPP
#define TOOLS_REPLIB_PRINTF_HPP

#include <kernel/types.hpp>

using namespace tacos::Kernel;

namespace tacos {
namespace Tools {
    namespace RepLib {
        void printf(u64 Num);
    }
}
}

#endif