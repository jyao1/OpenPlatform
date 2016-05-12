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
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  DSC_SPECIFICATION              = 0x00010005
  PLATFORM_NAME                  = Quark
  PLATFORM_GUID                  = F6E7730E-0C7A-4741-9DFC-6BC8B86CD865
  PLATFORM_VERSION               = 0.1
  FLASH_DEFINITION               = QuarkPlatformPkg/Quark.fdf
  OUTPUT_DIRECTORY               = Build/Quark
  SUPPORTED_ARCHITECTURES        = IA32
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  VPD_TOOL_GUID                  = 8C3D856A-9BE6-468E-850A-24F7A8D38E08

!include QuarkPlatformPkg/QuarkPlatformConfig.dsc

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

!include QuarkPlatformPkg/Include/Build/CorePkgInclude.dsc
!include QuarkPlatformPkg/Include/Build/CorePkgConfig.dsc
!include QuarkPlatformPkg/Include/Build/BuildOption.dsc

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses]

!if $(SECURE_BOOT_ENABLE)
  PlatformSecureLib|QuarkPlatformPkg/Library/PlatformSecureLib/PlatformSecureLib.inf
!endif

!if $(MEASURED_BOOT_ENABLE)
!if $(TPM_12_HARDWARE) == ATMEL_I2C
  Tpm12DeviceLib|QuarkPlatformPkg/Library/Tpm12DeviceLibAtmelI2c/Tpm12DeviceLibAtmelI2c.inf
!endif
!if $(TPM_12_HARDWARE) == INFINEON_I2C
  Tpm12DeviceLib|QuarkPlatformPkg/Library/Tpm12DeviceLibInfineonI2c/Tpm12DeviceLibInfineonI2c.inf
!endif
!endif

  #
  # CPU
  #
  MtrrLib|QuarkSocPkg/QuarkNorthCluster/Library/MtrrLib/MtrrLib.inf
  SmmCpuFeaturesLib|QuarkSocPkg/QuarkNorthCluster/Library/SmmCpuFeaturesLib/SmmCpuFeaturesLib.inf

  #
  # Quark North Cluster
  #
  SmmLib|QuarkSocPkg/QuarkNorthCluster/Library/QNCSmmLib/QNCSmmLib.inf
  SmbusLib|QuarkSocPkg/QuarkNorthCluster/Library/SmbusLib/SmbusLib.inf
  ResetSystemLib|QuarkSocPkg/QuarkNorthCluster/Library/ResetSystemLib/ResetSystemLib.inf
  IntelQNCLib|QuarkSocPkg/QuarkNorthCluster/Library/IntelQNCLib/IntelQNCLib.inf
  QNCAccessLib|QuarkSocPkg/QuarkNorthCluster/Library/QNCAccessLib/QNCAccessLib.inf
  ResetSystemLib|QuarkSocPkg/QuarkNorthCluster/Library/ResetSystemLib/ResetSystemLib.inf

  #
  # Quark South Cluster
  #
  IohLib|QuarkSocPkg/QuarkSouthCluster/Library/IohLib/IohLib.inf
  I2cLib|QuarkSocPkg/QuarkSouthCluster/Library/I2cLib/I2cLib.inf

  #
  # Quark Platform
  #
  RecoveryOemHookLib|QuarkPlatformPkg/Library/RecoveryOemHookLib/RecoveryOemHookLib.inf
  PlatformSecLib|QuarkPlatformPkg/Library/PlatformSecLib/PlatformSecLib.inf
  PlatformPcieHelperLib|QuarkPlatformPkg/Library/PlatformPcieHelperLib/PlatformPcieHelperLib.inf
  PlatformHelperLib|QuarkPlatformPkg/Library/PlatformHelperLib/DxePlatformHelperLib.inf
#!if $(GALILEO) == GEN2
  Pcal9555Lib|QuarkPlatformPkg/Board/GalileoGen2/Library/Pcal9555Lib/Pcal9555Lib.inf
#!endif

[LibraryClasses.IA32.PEIM,LibraryClasses.IA32.PEI_CORE]
  #
  # PEI phase common
  #
  PlatformHelperLib|QuarkPlatformPkg/Library/PlatformHelperLib/PeiPlatformHelperLib.inf

[LibraryClasses.IA32.DXE_RUNTIME_DRIVER]
  QNCAccessLib|QuarkSocPkg/QuarkNorthCluster/Library/QNCAccessLib/RuntimeQNCAccessLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag]
!if $(TARGET) == "RELEASE"
  gQuarkPlatformTokenSpaceGuid.WaitIfResetDueToError|FALSE
!else
  gQuarkPlatformTokenSpaceGuid.WaitIfResetDueToError|TRUE
!endif

[PcdsFixedAtBuild]
!if $(GALILEO) == GEN1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|460800
!endif
!if $(GALILEO) == GEN2
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|921600
!endif
  gQuarkPlatformTokenSpaceGuid.PcdEsramStage1Base|0x80000000

!if $(GALILEO) == GEN1
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate|460800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
!endif
!if $(GALILEO) == GEN2
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate|921600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
!endif


[PcdsDynamicExHii.common.DEFAULT]
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdBootState|L"BootState"|gQuarkPlatformTokenSpaceGuid|0x0|TRUE

[PcdsDynamicExDefault.common.DEFAULT]
  gQuarkPlatformTokenSpaceGuid.PcdEnableFastBoot|TRUE
  gQuarkPlatformTokenSpaceGuid.PcdUserIsPhysicallyPresent|FALSE
  gQuarkPlatformTokenSpaceGuid.PcdSpiFlashDeviceSize|0

[PcdsDynamicExVpd]
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|*|32|L"EDK II"
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareRevision|*|0x01000400
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|*|64|L"Galileo 1.0.4"

#
# ClantonPeakSVP
# gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0002
# gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"ClantonPeakSVP"
# gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x03, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
#
# KipsBay
# gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0003
# gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"KipsBay"
# gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
#
# CrossHill
# gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0004
# gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"CrossHill"
# gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x03, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
#
# ClantonHill
# gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0005
# gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"ClantonHill"
# gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x02, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
#
# Galileo
# gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0006
# gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"Galileo"
# gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
#
# GalileoGen2
# gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0008
# gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"GalileoGen2"
# gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
#
!if $(GALILEO) == GEN1
  gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0006
  gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"Galileo"
  gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
!endif
!if $(GALILEO) == GEN2
  gQuarkPlatformTokenSpaceGuid.PcdPlatformType|*|0x0008
  gQuarkPlatformTokenSpaceGuid.PcdPlatformTypeName|*|64|L"GalileoGen2"
  gEfiQuarkNcSocIdTokenSpaceGuid.PcdMrcParameters|*|40|{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x7c, 0x92, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x40, 0x9c, 0x00, 0x00, 0x06}
!endif
  gEfiQuarkSCSocIdTokenSpaceGuid.PcdIohEthernetMac0|*|8|{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
  gEfiQuarkSCSocIdTokenSpaceGuid.PcdIohEthernetMac1|*|8|{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}

[PcdsDynamicExDefault.common.DEFAULT]
  # Value to be fixed in each board
  gQuarkPlatformTokenSpaceGuid.PcdPciExpPerstResumeWellGpio|0
  gQuarkPlatformTokenSpaceGuid.PcdFlashUpdateLedResumeWellGpio|0

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
  QuarkSocPkg/QuarkNorthCluster/MemoryInit/Pei/MemoryInitPei.inf
#!if $(GALILEO) == GEN1
  QuarkPlatformPkg/Board/Galileo/BoardEarlyInit/BoardEarlyInit.inf
#!endif
#!if $(GALILEO) == GEN2
  QuarkPlatformPkg/Board/GalileoGen2/BoardEarlyInit/BoardEarlyInit.inf
#!endif
  QuarkPlatformPkg/PlatformInit/PlatformInitPei/PlatformEarlyInit.inf

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # S3
  #
  QuarkSocPkg/QuarkNorthCluster/Smm/Pei/SmmAccessPei/SmmAccessPei.inf
  QuarkSocPkg/QuarkNorthCluster/Smm/Pei/SmmControlPei/SmmControlPei.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # Recovery
  #
  QuarkSocPkg/QuarkSouthCluster/Usb/Common/Pei/UsbPei.inf
  QuarkSocPkg/QuarkSouthCluster/Usb/Ohci/Pei/OhciPei.inf
!endif

[Components.IA32]

  QuarkPlatformPkg/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf
#!if $(GALILEO) == GEN1
  QuarkPlatformPkg/Board/Galileo/BoardInit/BoardInitDxe.inf
#!endif
#!if $(GALILEO) == GEN2
  QuarkPlatformPkg/Board/GalileoGen2/BoardInit/BoardInitDxe.inf
#!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  QuarkPlatformPkg/Flash/SpiFvbServices/PlatformSpi.inf
  QuarkPlatformPkg/Flash/SpiFvbServices/PlatformSmmSpi.inf
!endif

  QuarkSocPkg/QuarkNorthCluster/QNCInit/Dxe/QNCInitDxe.inf

!if $(BOOT_SHELL_ONLY) == FALSE
  QuarkSocPkg/QuarkNorthCluster/Smm/Dxe/SmmAccessDxe/SmmAccess.inf
  QuarkSocPkg/QuarkNorthCluster/Spi/PchSpiRuntime.inf {
    <LibraryClasses>
      PciExpressLib|MdePkg/Library/DxeRuntimePciExpressLib/DxeRuntimePciExpressLib.inf
  }
  QuarkSocPkg/QuarkNorthCluster/Spi/PchSpiSmm.inf
  QuarkSocPkg/QuarkNorthCluster/S3Support/Dxe/QncS3Support.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # ACPI
  #
  QuarkPlatformPkg/Acpi/AcpiPlatform/AcpiPlatform.inf
#!if $(GALILEO) == GEN1
  QuarkPlatformPkg/Board/Galileo/AcpiTables/AcpiBoard.inf
#!endif
#!if $(GALILEO) == GEN2
  QuarkPlatformPkg/Board/GalileoGen2/AcpiTables/AcpiBoard.inf
#!endif
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # SMM
  #
  QuarkSocPkg/QuarkNorthCluster/Smm/Dxe/SmmControlDxe/SmmControlDxe.inf
  QuarkSocPkg/QuarkNorthCluster/Smm/DxeSmm/QncSmmDispatcher/QNCSmmDispatcher.inf
  QuarkPlatformPkg/Acpi/AcpiSmm/AcpiSmmPlatform.inf
  QuarkPlatformPkg/Feature/PowerManagement/SleepSmm/SleepSmmPlatform.inf
  QuarkPlatformPkg/Feature/PowerManagement/CpuPowerManagement/SmmPowerManagement.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # SMBIOS
  #
  QuarkPlatformPkg/Feature/Smbios/SmbiosMiscDxe/SmbiosMiscDxe.inf
  QuarkPlatformPkg/Feature/Smbios/MemorySubClass/MemorySubClass.inf
!endif

  #
  # PCI
  #
  QuarkPlatformPkg/Pci/Dxe/PciPlatform/PciPlatform.inf
  QuarkSocPkg/QuarkSouthCluster/IohInit/Dxe/IohInitDxe.inf

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # USB
  #
  QuarkSocPkg/QuarkSouthCluster/Usb/Ohci/Dxe/OhciDxe.inf
!endif

!if $(BOOT_SHELL_ONLY) == FALSE
  #
  # SDIO
  #
  QuarkSocPkg/QuarkSouthCluster/Sdio/Dxe/SDControllerDxe/SDControllerDxe.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80300087
  }
  QuarkSocPkg/QuarkSouthCluster/Sdio/Dxe/SDMediaDeviceDxe/SDMediaDeviceDxe.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80300087
  }
!endif
