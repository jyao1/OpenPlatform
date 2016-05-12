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
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses]
  #
  # Entry point
  #
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf

  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  PciHostBridgeLib|QuarkPlatformPkg/Override/MdeModulePkg/Library/PciHostBridgeLibSimple/PciHostBridgeLibSimple.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
!if $(SOURCE_DEBUG_ENABLE)
  PeCoffExtraActionLib|SourceLevelDebugPkg/Library/PeCoffExtraActionLibDebug/PeCoffExtraActionLibDebug.inf
  DebugCommunicationLib|SourceLevelDebugPkg/Library/DebugCommunicationLibSerialPort/DebugCommunicationLibSerialPort.inf
!else
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
!endif
  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf

  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiDecompressLib|IntelFrameworkModulePkg/Library/BaseUefiTianoCustomDecompressLib/BaseUefiTianoCustomDecompressLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiCpuLib|UefiCpuPkg/Library/BaseUefiCpuLib/BaseUefiCpuLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf

  #
  # Generic Modules
  #
!if $(BOOT_SHELL_ONLY)
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
!else
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
!endif
  S3IoLib|MdePkg/Library/BaseS3IoLib/BaseS3IoLib.inf
  S3PciLib|MdePkg/Library/BaseS3PciLib/BaseS3PciLib.inf
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  IpIoLib|MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf
  UdpIoLib|MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  DpcLib|MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  SmmCorePlatformHookLib|MdeModulePkg/Library/SmmCorePlatformHookLibNull/SmmCorePlatformHookLibNull.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
!if $(BOOT_SHELL_ONLY)
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
!else
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
!endif
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
!if $(BOOT_SHELL_ONLY)
  LockBoxLib|MdeModulePkg/Library/LockBoxNullLib/LockBoxNullLib.inf
!else
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxDxeLib.inf
!endif
  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
!if $(LOGGING)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!endif
!if $(PERFORMANCE_ENABLE)
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
!else
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
!endif

!if $(SECURE_BOOT_ENABLE) || $(MEASURED_BOOT_ENABLE)
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
!endif

!if $(SECURE_BOOT_ENABLE)
  AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
!else
  AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
!endif

!if $(MEASURED_BOOT_ENABLE)
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  Tpm12CommandLib|SecurityPkg/Library/Tpm12CommandLib/Tpm12CommandLib.inf
!if $(TPM_12_HARDWARE) == LPC
  Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf
!endif
  TcgPpVendorLib|SecurityPkg/Library/TcgPpVendorLibNull/TcgPpVendorLibNull.inf
!else
  TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
!endif

  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf

  #
  # CPU
  #
  LocalApicLib|UefiCpuPkg/Library/BaseXApicLib/BaseXApicLib.inf

  #
  # Silicon
  #
  TimerLib|PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  IoApicLib|PcAtChipsetPkg/Library/BaseIoApicLib/BaseIoApicLib.inf

  SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
  PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf

[LibraryClasses.common.SEC]
  #
  # SEC specific phase
  #
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  TimerLib|PcAtChipsetPkg/Library/AcpiTimerLib/BaseAcpiTimerLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf

[LibraryClasses.IA32.PEIM,LibraryClasses.IA32.PEI_CORE]
  #
  # PEI phase common
  #
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
!if $(BOOT_SHELL_ONLY)
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
!else
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
!endif
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
!if $(BOOT_SHELL_ONLY)
  LockBoxLib|MdeModulePkg/Library/LockBoxNullLib/LockBoxNullLib.inf
!else
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxPeiLib.inf
!endif
  TimerLib|PcAtChipsetPkg/Library/AcpiTimerLib/BaseAcpiTimerLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf
!if $(SECURE_BOOT_ENABLE) || $(MEASURED_BOOT_ENABLE)
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf
!endif
!if $(PERFORMANCE_ENABLE)
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
!endif

[LibraryClasses.IA32.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf
!if $(PERFORMANCE_ENABLE)
  PerformanceLib|MdeModulePkg/Library/DxeCorePerformanceLib/DxeCorePerformanceLib.inf
!endif

[LibraryClasses.IA32.DXE_SMM_DRIVER]
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
!if $(BOOT_SHELL_ONLY)
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
!else
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
!endif
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
!if $(BOOT_SHELL_ONLY)
  LockBoxLib|MdeModulePkg/Library/LockBoxNullLib/LockBoxNullLib.inf
!else
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxSmmLib.inf
!endif
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SmmCpuExceptionHandlerLib.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
!if $(SECURE_BOOT_ENABLE) || $(MEASURED_BOOT_ENABLE)
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
!endif
!if $(PERFORMANCE_ENABLE)
  PerformanceLib|MdeModulePkg/Library/SmmPerformanceLib/SmmPerformanceLib.inf
!endif

[LibraryClasses.IA32.SMM_CORE]
  SmmServicesTableLib|MdeModulePkg/Library/PiSmmCoreSmmServicesTableLib/PiSmmCoreSmmServicesTableLib.inf
!if $(BOOT_SHELL_ONLY)
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
!else
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
!endif
  MemoryAllocationLib|MdeModulePkg/Library/PiSmmCoreMemoryAllocationLib/PiSmmCoreMemoryAllocationLib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
!if $(SECURE_BOOT_ENABLE) || $(MEASURED_BOOT_ENABLE)
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
!endif
!if $(PERFORMANCE_ENABLE)
  PerformanceLib|MdeModulePkg/Library/SmmCorePerformanceLib/SmmCorePerformanceLib.inf
!endif

[LibraryClasses.IA32.DXE_RUNTIME_DRIVER]
!if $(BOOT_SHELL_ONLY)
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
!else
  ReportStatusCodeLib|MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
!endif
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
!if $(SECURE_BOOT_ENABLE) || $(MEASURED_BOOT_ENABLE)
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/RuntimeCryptLib.inf
!endif

[LibraryClasses.IA32.UEFI_DRIVER,LibraryClasses.IA32.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary, FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################

[Components.IA32]
  #
  # SEC Core
  #
  UefiCpuPkg/SecCore/SecCore.inf {
    !if $(SOURCE_DEBUG_ENABLE)
      <LibraryClasses>
        PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
        DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SecPeiDebugAgentLib.inf
    !endif
  }

  #
  # PEI Core
  #
  MdeModulePkg/Core/Pei/PeiMain.inf

  #
  # PEIM
  #
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
!if $(BOOT_SHELL_ONLY) == FALSE
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf {
    <LibraryClasses>
      !if $(LOGGING)
        DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      !else
        SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
      !endif
  }
!endif

  MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf
  MdeModulePkg/Universal/Variable/Pei/VariablePei.inf
  MdeModulePkg/Universal/PcatSingleSegmentPciCfg2Pei/PcatSingleSegmentPciCfg2Pei.inf
!if $(BOOT_SHELL_ONLY) == FALSE
  UefiCpuPkg/CpuMpPei/CpuMpPei.inf
  MdeModulePkg/Universal/CapsulePei/CapsulePei.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationPei.inf
!endif

  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # S3
  #
  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf
!endif

  #
  # Trusted Platform Module
  #
!if $(MEASURED_BOOT_ENABLE)
  SecurityPkg/Tcg/TrEEConfig/TrEEConfigPei.inf
  SecurityPkg/Tcg/TcgPei/TcgPei.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # Recovery
  #
  MdeModulePkg/Bus/Pci/EhciPei/EhciPei.inf
  MdeModulePkg/Bus/Usb/UsbBotPei/UsbBotPei.inf
  MdeModulePkg/Bus/Usb/UsbBusPei/UsbBusPei.inf
  FatPkg/FatPei/FatPei.inf
  MdeModulePkg/Universal/Disk/CdExpressPei/CdExpressPei.inf
!endif

[Components.IA32]
  #
  # DXE Core
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf

!if $(SOURCE_DEBUG_ENABLE)
  SourceLevelDebugPkg/DebugAgentDxe/DebugAgentDxe.inf {
    <LibraryClasses>
      DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
  }
!endif

  #
  # Components that produce the architectural protocols
  #
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf  {
    <LibraryClasses>
!if $(SECURE_BOOT_ENABLE)
      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
!endif
 }
  UefiCpuPkg/CpuDxe/CpuDxe.inf
  MdeModulePkg/Universal/Metronome/Metronome.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
!if $(BOOT_SHELL_ONLY)
  MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf
!else
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteSmm.inf
!if $(SECURE_BOOT_ENABLE)
  SecurityPkg/VariableAuthenticated/SecureBootConfigDxe/SecureBootConfigDxe.inf
!endif
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmmRuntimeDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmm.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      NULL|MdeModulePkg/Library/VarCheckHiiLib/VarCheckHiiLib.inf
      NULL|MdeModulePkg/Library/VarCheckPcdLib/VarCheckPcdLib.inf
  }
!endif

  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf {
    <LibraryClasses>
!if $(BOOT_SHELL_ONLY)
      CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
!else
      GenericBdsLib|IntelFrameworkModulePkg/Library/GenericBdsLib/GenericBdsLib.inf
      CapsuleLib|IntelFrameworkModulePkg/Library/DxeCapsuleLib/DxeCapsuleLib.inf
!endif
  }
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  PcAtChipsetPkg/PcatRealTimeClockRuntimeDxe/PcatRealTimeClockRuntimeDxe.inf

  #
  # Following are the DXE drivers (alphabetical order)
  #
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

#  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf
  QuarkPlatformPkg/Override/MdeModulePkg/Bus/Pci/PciHostBridge/PciHostBridge.inf
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf

  #
  # Platform
  #
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
      UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
      PlatformBootManagerLib|QuarkPlatformPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  }
!if $(BOOT_SHELL_ONLY) == FALSE
  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf

      UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
!endif

  PcAtChipsetPkg/HpetTimerDxe/HpetTimerDxe.inf

!if $(BOOT_SHELL_ONLY) == FALSE
  MdeModulePkg/Universal/SectionExtractionDxe/SectionExtractionDxe.inf
!endif

  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf

!if $(BOOT_SHELL_ONLY) == FALSE
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf  {
    <LibraryClasses>
      !if $(LOGGING)
        DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      !else
        SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
      !endif
  }
  MdeModulePkg/Universal/ReportStatusCodeRouter/Smm/ReportStatusCodeRouterSmm.inf
  MdeModulePkg/Universal/StatusCodeHandler/Smm/StatusCodeHandlerSmm.inf {
    <LibraryClasses>
      !if $(LOGGING)
        DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      !else
        SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
      !endif
  }
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # ACPI
  #
  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf
#  MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf {
  QuarkPlatformPkg/Override/MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf {
    <LibraryClasses>
      !if $(LOGGING)
        DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      !else
        SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
      !endif
      !if $(SOURCE_DEBUG_ENABLE)
        DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
      !endif
  }
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # SMM
  #
  MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf
  MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <LibraryClasses>
      SmmCpuPlatformHookLib|UefiCpuPkg/Library/SmmCpuPlatformHookLibNull/SmmCpuPlatformHookLibNull.inf

      !if $(LOGGING)
        DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      !else
        SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
      !endif
      !if $(SOURCE_DEBUG_ENABLE)
        DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SmmDebugAgentLib.inf
      !endif
    <PcdsPatchableInModule>
      #
      # Disable DEBUG_CACHE because SMI entry/exit may change MTRRs
      #
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x801000C7
  }

  UefiCpuPkg/CpuIo2Smm/CpuIo2Smm.inf
  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationSmm.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # SMBIOS
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
!endif

  #
  # PCI
  #
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  MdeModulePkg/Bus/Pci/PciSioSerialDxe/PciSioSerialDxe.inf

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # USB
  #
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
!endif

  #
  # Console
  #
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf {
    <LibraryClasses>
      CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf
  }
!if $(BOOT_SHELL_ONLY) == FALSE
  QuarkPlatformPkg/Override/MdeModulePkg/Universal/KeyboardLayout/KeyboardLayoutDxe.inf
!endif
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  #
  # File System Modules
  #
!if $(BOOT_SHELL_ONLY) == FALSE
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
!endif
!if $(PERFORMANCE_ENABLE) || $(BOOT_SHELL_ONLY)
  MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # Capsule update
  #
  IntelFrameworkModulePkg/Universal/FirmwareVolume/FwVolDxe/FwVolDxe.inf
  IntelFrameworkModulePkg/Universal/FirmwareVolume/UpdateDriverDxe/UpdateDriverDxe.inf
!endif

  #
  # Trusted Platform Module
  #
!if $(MEASURED_BOOT_ENABLE)
  SecurityPkg/Tcg/MemoryOverwriteControl/TcgMor.inf
  SecurityPkg/Tcg/TcgDxe/TcgDxe.inf
  SecurityPkg/Tcg/TcgSmm/TcgSmm.inf
!endif

  #
  # Performance Application
  #
!if $(PERFORMANCE_ENABLE)
  PerformancePkg/Dp_App/Dp.inf {
    <LibraryClasses>
      ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
      FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  }
!endif

  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
      FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf

    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }
