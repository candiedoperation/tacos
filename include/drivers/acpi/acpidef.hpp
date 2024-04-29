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

#ifndef DRIVERS_ACPI_HPP
#define DRIVERS_ACPI_HPP

#include <kernel/types.hpp>

using namespace tacos::Kernel;

#define ACPI_EBDA_SEG 0x40E
#define ACPI_BIOS_MEM_STA 0x000E0000
#define ACPI_BIOS_MEM_END 0x000FFFFF
#define ACPI_RSDP_STLEN 20 /* Length of the RSDP Struct */
#define ACPI_XSDP_STLEN 36 /* Length of the XSDP Struct */

#define ACPI_SIG_RSDP "RSD PTR "
#define ACPI_SIG_FADT "FACP"
#define ACPI_SIG_MADT "APIC"

namespace tacos {
namespace Drivers {
    namespace Acpi {
        /// @brief ACPI Definitions Class
        class AcpiDef {
        public:
            /// @brief XSDP Table Address
            typedef u8* RSDPAddress;
            typedef u32 Status;
            typedef u64 Address;

            /// @brief ACPI Versions and their Revision Code
            enum Version {
                INVALID = 0,
                ONE = 1,
                TWO = 2
            };

            /// @brief ACPI Power Management Profiles
            enum PowerManagementProfile {
                UNSPECIFIED = 0,
                DESKTOP = 1,
                MOBILE = 2,
                WORKSTATION = 3,
                ENTERPRISE_SERVER = 4,
                SOHO_SERVER = 5,
                APPLIANCE_PC = 6,
                PERFORMANCE_SERVER = 7,
                TABLET = 8
            };

            enum MadtEntryType {
                LOCAL_APIC = 0,
            };

            /// @brief Common Header for all ACPI System Descriptor Tables
            struct SdtHeader {
                char Signature[4];
                u32 Length;
                u8 Revision;
                u8 Checksum;
                char OEMID[6];
                char OEMTableID[8];
                u32 OEMRevision;
                u32 CreatorID;
                u32 CreatorRevision;
            } __attribute__((packed));

            /// @brief MADT Variable Length Entry
            struct MadtEntryHeader {
                u8 EntryType;
                u8 RecordLength;
            } __attribute__((packed));

            /// @brief Generic Address Strucutre (GAS) Format
            struct GenericAddressStructure {
                /*
                    ACPI Uses the Generic Address Structure format
                    to describe the position of registers. The str
                    -ucture is 12 bytes in length.

                    Refer:
                    https://wiki.osdev.org/FADT#GenericAddressStructure
                */

                u8 AddressSpace;
                u8 BitWidth;
                u8 BitOffset;
                u8 AccessSize;
                u64 Address;
            };

            /// @brief The Root System Descriptor Pointer Structure
            struct Rsdp {
                /*
                    The Root System Description Pointer (RSDP) structure is located in the
                    system’s memory address space and is setup by the platform firmware. The
                    RSDP consists of a pointer to the Extended System Description Table, XSDT,
                    which contains an array of pointers to all other System Description Tables.

                    Refer:
                    https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/05_ACPI_Software_Programming_Model/ACPI_Software_Programming_Model.html
                */

                char Signature[8];
                u8 Checksum;
                char OEMID[6];
                u8 Revision;
                u32 RsdtAddress;

                u32 Length;
                u64 XsdtAddress;
                u8 ExtendedChecksum;
                u8 Reserved[3];
            } __attribute__((packed));

            /// @brief Root System Description Table Structure
            struct Rsdt {
                /* Refer: https://wiki.osdev.org/RSDT */
                SdtHeader Header;
                u32 SdtList[1];
            };

            /// @brief Extended System Descriptor Table Structure
            struct Xsdt {
                /*
                    The XSDT is the main System Description Table. However there are
                    many kinds of SDT. All the SDT may be split into two parts. One
                    (the header) which is common to all the SDT an another (data) which
                    is different for each table.

                    The SdtList consists of pointers to all available SDTs. The size is
                    set to one as it's a hack (struct hack) that lets initialize the
                    array and access elements at any offset. In case of invalid access,
                    it leads to undefined behavior.

                    The SdtList field should be defined such that it is aligned to a
                    4-byte boundary and not the default 8-byte alignment for a uint64_t.
                    Also, The offset to the first pointer from the beginning of the XSDT
                    table is 36 bytes.

                    Refer:
                    https://wiki.osdev.org/XSDT#Structure
                */

                SdtHeader Header;
                u64 SdtList[1] __attribute__((aligned(4)));
            } __attribute__((packed));

            /// @brief Fixed ACPI Description Table Structure
            struct Fadt {
                /*
                    The Fixed ACPI Description Table (FADT) defines various fixed
                    hardware ACPI information vital to an ACPI-compatible OS. The
                    FADT also has a pointer to the DSDT that contains the Differen
                    -tiated Definition Block.

                    Refer:
                    https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/05_ACPI_Software_Programming_Model/ACPI_Software_Programming_Model.html#fixed-acpi-description-table-fadt
                    https://wiki.osdev.org/FADT
                */

                SdtHeader Header;
                u32 FirmwareCtrl;
                u32 Dsdt; /* Physical Memory Address of DSDT */
                u8 Reserved0;

                u8 PreferredPowerMgmtProfile; /* Map to PwrMgmt Enum */
                u16 SciInterrupt;
                u32 SmiCommandPort;
                u8 AcpiEnable;
                u8 AcpiDisable;
                u8 S4BiosReq;
                u8 PerformanceStateControl;

                /* Power Management (PM) Register Blocks */
                u32 Pm1aEventBlock;
                u32 Pm1bEventBlock;
                u32 Pm1aControlBlock;
                u32 Pm1bControlBlock;
                u32 Pm2ControlBlock;
                u32 PmTimerBlock;

                /* General Purpose Event (GPE) Register Blocks */
                u32 Gpe0Block;
                u32 Gpe1Block;

                u8 Pm1EventLength; /* Bytes decoded by Pm1EventBlock. Always >= 4 */
                u8 Pm1ControlLength; /* Bytes decoded by Pm1ControlBlock. Always >= 2 */
                u8 Pm2ControlLength; /* Bytes decoded, If Pm2Control supported, >= 1 */
                u8 PmTimerLength; /* If Timer supported, value is 4 otherwise zero */
                u8 Gpe0BlockLength; /* Bytes Decoded by Gpe0Block. Multiple of 2 */
                u8 Gpe1BlockLength; /* Bytes Decoded by Gpe1Block. Multiple of 2 */
                u8 Gpe1Base;

                /* CPU Power Profile (C State) Fields */
                u8 CStateControl;
                u16 CState2SwitchLatency; /* Worst Latency in toggling C2 State */
                u16 CState3SwitchLatency; /* Worst Latency in toggling C3 State */

                u16 FlushSize;
                u16 FlushStride;
                u8 DutyOffset;
                u8 DutyWidth;
                u8 DayOfMonthAlarm;
                u8 MonthOfYearAlarm;
                u8 Century; /* RTC Century Feature */

                u16 IAPCBootArchFlags; /* IA-PC Boot Architecture Flags */
                u8 Reserved1;
                u32 Flags;

                /* ACPI System Reset Fields */
                GenericAddressStructure ResetRegister;
                u8 ResetRegisterValue;

                u16 ArmBootArchFlags; /* ARM Boot Architecture Flags */
                u8 FadtMinorVersion;

                /* Extended 64bit Pointers */
                u64 ExFirmwareControl;
                u64 ExDsdt;

                /* Extended Power Manamgement (PM) Register Blocks */
                GenericAddressStructure ExPm1aEventBlock;
                GenericAddressStructure ExPm1bEventBlock;
                GenericAddressStructure ExPm1aControlBlock;
                GenericAddressStructure ExPm1bControlBlock;
                GenericAddressStructure ExPm2ControlBlock;
                GenericAddressStructure ExPmTimerBlock;

                /* Extended General Purpose Event (GPE) Register Blocks */
                GenericAddressStructure ExGpe0Block;
                GenericAddressStructure ExGpe1Block;

                /* Other Extended Registers */
                GenericAddressStructure SleepControlRegister;
                GenericAddressStructure SleepStatusRegister;
                u64 HypervisorVendorId;
            } __attribute__((packed));

            /// @brief Multiple APIC Descriptor Table Structure
            struct Madt {
                /*
                    The Multiple APIC Description Table (MADT) is an ACPI table which
                    provides informations necessary for operation on systems with APIC.
                    The last part of the structure is a list of Interrupt Controller
                    structures. The first byte defines the structure type and the second
                    byte defines the structure length.

                    Refer:
                    https://wiki.osdev.org/MADT
                    https://blog.wesleyac.com/posts/ioapic-interrupts
                    https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/05_ACPI_Software_Programming_Model/ACPI_Software_Programming_Model.html#multiple-apic-description-table-madt
                */

                SdtHeader Header;
                u32 LocalAPICAddr;
                u32 Flags;
                u8 InterruptControllersLoc;
            } __attribute__((packed));

            /// @brief Processor Local APIC Entry Type
            struct MadtEntryLocalApic {
                MadtEntryHeader Header;
                u8 AcpiProcessorId;
                u8 ApicId;
                u32 Flags;
            } __attribute__((packed));

            /// @brief I/O APIC Entry Type
            struct MadtEntryApic {
                MadtEntryHeader Header;
                u8 ApicId;
                u8 Reserved;
                u32 ApicAddress;
                u32 GsiBase; /* What's this? */
            } __attribute__((packed));

            /// @brief I/O APIC Interrupt Source Override Entry
            struct MadtEntryApicISROverride {
                MadtEntryHeader Header;
                u8 BusSource;
                u8 IrqSource;
                u32 Gsi;
                u16 Flags;    
            } __attribute__((packed));

            /// @brief I/O APIC Non-maskable Interrupt Source Entry
            struct MadtEntryApicNMISource {
                MadtEntryHeader Header;
                u8 NmiSource;
                u8 Reserved;
                u16 Flags;
                u32 Gsi;
            } __attribute__((packed));

            /// @brief Local APIC Non-maskable Interrupt Entry
            struct MadtEntryLocalApicNMI {
                MadtEntryHeader Header;
                u8 AcpiProcessorId;
                u16 Flags;
                u8 LINT;
            } __attribute__((packed));

            /// @brief Local APIC Address Override Entry
            struct MadtEntryLocalApicAddrOverride {
                MadtEntryHeader Header;
                u16 Reserved;
                u64 PhysicalAddress;
            } __attribute__((packed));

            /// @brief Processor Local x2APIC Entry
            struct MadtEntryLocalX2Apic {
                MadtEntryHeader Header;
                u16 Reserved;
                u32 ApicId;
                u32 Flags;
                u32 AcpiId;
            } __attribute__((packed));

            static RSDPAddress GetRSDPAddr();
            static Version GetACPIVersion(const Rsdp* XsdpTbl);
            static Status GetTableBySignature(char* Signature, Xsdt* Xsdt, Address* Table);
        };
    }
}
}

#endif