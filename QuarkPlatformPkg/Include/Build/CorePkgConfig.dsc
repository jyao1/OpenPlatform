## @file
# Clanton Peak CRB platform with 32-bit DXE for 4MB/8MB flash devices.
#
# This package provides Clanton Peak CRB platform specific modules.
# Copyright (c) 2013 - 2016 Intel Corporation.
#
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
##

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag]
  gEfiMdeModulePkgTokenSpaceGuid.PcdDevicePathSupportDevicePathToText|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDevicePathSupportDevicePathFromText|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciBusHotplugDeviceSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE
!if $(SOURCE_DEBUG_ENABLE)
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmDebug|TRUE
!endif

[PcdsFixedAtBuild]
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|1
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0x20000
!if $(BOOT_SHELL_ONLY)
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|TRUE
!endif
!if $(LOGGING)
  !if $(SOURCE_DEBUG_ENABLE)
    gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x17
  !else
    gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x27
  !endif
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdPostCodePropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!endif
  gEfiMdePkgTokenSpaceGuid.PcdPostCodePropertyMask|0x18
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xE0000000
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|0
!if $(PERFORMANCE_ENABLE)
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|80
!else
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x00
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|0
!if $(SECURE_BOOT_ENABLE)
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x2000
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x00002000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x1000
  ## RTC Update Timeout Value, need to increase timeout since also
  # waiting for RTC to be busy.
  gEfiMdeModulePkgTokenSpaceGuid.PcdRealTimeClockUpdateTimeout|500000

!if $(SECURE_BOOT_ENABLE)
  # override the default values from SecurityPkg to ensure images from all sources are verified in secure boot
  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy|0x04
  gEfiSecurityPkgTokenSpaceGuid.PcdFixedMediaImageVerificationPolicy|0x04
  gEfiSecurityPkgTokenSpaceGuid.PcdRemovableMediaImageVerificationPolicy|0x04
!endif

!if $(SOURCE_DEBUG_ENABLE)
  gEfiSourceLevelDebugPkgTokenSpaceGuid.PcdDebugLoadImageMethod|0x2
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseMmio|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x9000B000
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialLineControl|0x03
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialFifoControl|0x07
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialDetectCable|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialClockRate|44236800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialPciDeviceInfo|{0x14, 0x05, 0x84, 0x00, 0xFF}
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterStride|4

  #
  #  typedef struct {
  #    UINT16  VendorId;          ///< Vendor ID to match the PCI device.  The value 0xFFFF terminates the list of entries.
  #    UINT16  DeviceId;          ///< Device ID to match the PCI device
  #    UINT32  ClockRate;         ///< UART clock rate.  Set to 0 for default clock rate of 1843200 Hz
  #    UINT64  Offset;            ///< The byte offset into to the BAR
  #    UINT8   BarIndex;          ///< Which BAR to get the UART base address
  #    UINT8   RegisterStride;    ///< UART register stride in bytes.  Set to 0 for default register stride of 1 byte.
  #    UINT16  ReceiveFifoDepth;  ///< UART receive FIFO depth in bytes. Set to 0 for a default FIFO depth of 16 bytes.
  #    UINT16  TransmitFifoDepth; ///< UART transmit FIFO depth in bytes. Set to 0 for a default FIFO depth of 16 bytes.
  #    UINT8   Reserved[2];
  #  } PCI_SERIAL_PARAMETER;
  #
  # Vendor 8086 Device 0936 Prog Interface 2, BAR #0, Offset 0, Stride = 4, Clock 44236800 (0x2a300000)
  # Vendor 8086 Device 0936 Prog Interface 2, BAR #0, Offset 0, Stride = 4, Clock 44236800 (0x2a300000)
  #
  #                                                       [Vendor]   [Device]  [---ClockRate---]  [------------Offset-----------] [Bar] [Stride] [RxFifo] [TxFifo]   [Rsvd]   [Vendor]
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciSerialParameters|{0x86,0x80, 0x36,0x09, 0x0,0x0,0xA3,0x02, 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, 0x00,    0x04, 0x0,0x0, 0x0,0x0, 0x0,0x0, 0xff,0xff}

  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber           |0
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber        |31
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber      |0
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset|0x4b
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask          |0x80
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset   |0x48
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress        |0x1000
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset             |0x0008

  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE

  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciReservedIobase   |0x2000
  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciReservedIoLimit  |0xFFFF
  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciReservedMemBase  |0x90000000
  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciReservedMemLimit |0xAFFFFFFF
  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciReservedMemAbove4GBBase  |0x0
  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciReservedMemAbove4GBLimit |0x0
  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciDmaAbove4G|FALSE
  #gEfiMdeModulePkgTokenSpaceGuid.PcdPciNoExtendedConfigSpace|FALSE

[PcdsPatchableInModule]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x803000C7
  #gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80080046
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress|0x0

[PcdsDynamicExHii.common.DEFAULT]
!if $(PERFORMANCE_ENABLE)
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|0
!else
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5
!endif

[PcdsDynamicExDefault.common.DEFAULT]
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0x0

!if $(MEASURED_BOOT_ENABLE)
  #
  # TPM1.2      { 0x8b01e5b6, 0x4f19, 0x46e8, { 0xab, 0x93, 0x1c, 0x53, 0x67, 0x1b, 0x90, 0xcc } }
  # TPM2.0 DTPM { 0x286bf25a, 0xc2c3, 0x408c, { 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17 } }
  #
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0xb6, 0xe5, 0x01, 0x8b, 0x19, 0x4f, 0xe8, 0x46, 0xab, 0x93, 0x1c, 0x53, 0x67, 0x1b, 0x90, 0xcc}
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmScrtmPolicy|1
!endif
