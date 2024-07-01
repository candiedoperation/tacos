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

#include <kernel/mem/virtualmm.hpp>
#include <drivers/acpi/acpipvdr.hpp>
#include <drivers/hal/apic.hpp>
#include <tools/kernelrtl/kernelrtl.hpp>

using namespace tacOS::Drivers::HAL;
using namespace tacOS::Drivers::Acpi;
using namespace tacOS::Tools::KernelRTL;

static Apic::Status ProcessApicISROverride(AcpiDef::MadtEntryApicISROverride* ApicISROverride)
{
}

/// @brief Initializes the Advanced Programmable Interrupt Controller (APIC)
Apic::Status Apic::Initialize()
{
    /*
        Enable APIC based interrupts by configuring the
        IOREDTBL entry. This configuration is obtained by
        parsing the Multiple APIC Descriptor Table (MADT).
        We're assuming ACPI is Initialized by LoadKernel().

        Refer:
        https://blog.wesleyac.com/posts/ioapic-interrupts
    */

    AcpiDef::Address MadtAddr;
    AcpiDef::GetTableBySignature(ACPI_SIG_MADT, AcpiProvider::Xsdt, &MadtAddr);
    AcpiDef::Madt* Madt = (AcpiDef::Madt*)MadtAddr;

    /* FUTURE: REPLACE WITH DEBUG STATEMENTS */
    printf("\nLocal APIC Address: 0x");
    printf(Madt->LocalAPICAddr, 16);
    printf("\n");

    /*
        The MADT table contains a sequence of variable length
        records to enumerate the interrupt devices on the mac
        -hine. Each record begins consists of a MADT Entry Hea
        -der. Once the EntryType is parsed from the header, we
        can perform actions appropriately.

        Refer:
        https://wiki.osdev.org/MADT
        https://github.com/pdoane/osdev/blob/master/acpi/acpi.c
        https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/05_ACPI_Software_Programming_Model/ACPI_Software_Programming_Model.html#multiple-apic-description-table-madt
    */

    AcpiDef::MadtEntryApic* IoApic;
    AcpiDef::MadtEntryLocalApic* LApicList[10];

    u8* MadtEntryPtr = (u8*) (Madt + 1);
    u8* MadtEnd = (u8*) Madt + Madt->Header.Length;

    while (MadtEntryPtr < MadtEnd) {
        AcpiDef::MadtEntryHeader* Header = (AcpiDef::MadtEntryHeader*) MadtEntryPtr;
        MadtEntryPtr += Header->RecordLength;

        switch(Header->EntryType) {
            case AcpiDef::MadtEntryType::LOCAL_APIC: {
                AcpiDef::MadtEntryLocalApic* LApic = (AcpiDef::MadtEntryLocalApic*) Header;
                LApicList[LApic->AcpiProcessorId] = LApic;

                printf("Processor Detected (CPUID): ");
                printf(LApic->AcpiProcessorId);
                printf("\n");
                break;
            }

            case AcpiDef::MadtEntryType::IO_APIC: {
                IoApic = (AcpiDef::MadtEntryApic*) Header;
                printf("IO/APIC Detected (APIC ID): ");
                printf(IoApic->ApicId);
                printf("\n");
                break;
            }

            case AcpiDef::MadtEntryType::IO_APIC_ISR_OVERRIDE: {
                AcpiDef::MadtEntryApicISROverride* IsrOverride = (AcpiDef::MadtEntryApicISROverride*) Header;
                printf("IO/APIC ISR Override (IRQ#): ");
                printf(IsrOverride->IrqSource);
                printf("\n");
                break;
            }

            default: {
                printf("Unknown MADT Entry Type ");
                printf(Header->EntryType);
                printf("\n");
                break;
            }
        }
    }

    /* Map IO/APIC to Virtual Address Space */
    u64* IoApicAddr = VirtualMemory::HardwareRemap((u64*) IoApic->ApicAddress);
    printf("\nIO/APIC Addr: 0x");
    printf((u64) IoApicAddr, 16);

    /* Write to IOREGSEL */
    *IoApicAddr = 0;

    return Status::OK;
}