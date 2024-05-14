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

#ifndef KERNEL_MBOOTPVDR_HPP
#define KERNEL_MBOOTPVDR_HPP

#include <kernel/multiboot/mbdef.hpp>
#include <kernel/types.hpp>

using namespace tacOS::Kernel;

namespace tacOS {
namespace Kernel {
    /// @brief Multiboot 2 Support Routines
    class MBootProvider {
    public:
        static MBootDef::MultibootInfo* MBootInfoPtr;
        static int Initialize(u64 MultibootInfoPtrAddress);
    };
}
}

#endif