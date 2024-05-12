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

#ifndef DRIVERS_ACPI_PVDR_HPP
#define DRIVERS_ACPI_PVDR_HPP

#include <drivers/acpi/acpidef.hpp>
#include <kernel/types.hpp>

using namespace tacOS::Kernel;

namespace tacOS {
namespace Drivers {
    namespace Acpi {
        class AcpiProvider {
        public:
            static AcpiDef::RSDPAddress RsdpAddr;
            static AcpiDef::Rsdp* Rsdp;
            static AcpiDef::Xsdt* Xsdt;

            static AcpiDef::Status Initialize();
        };
    }
}
}

#endif