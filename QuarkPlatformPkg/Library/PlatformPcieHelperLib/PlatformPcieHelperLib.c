/** @file
Platform Pcie Helper Lib.

Copyright (c) 2013 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"

//
// Routines local to this source module.
//
VOID
LegacyGpioSetLevel (
  IN CONST UINT32                         LevelRegOffset,
  IN CONST UINT32                         GpioNum,
  IN CONST BOOLEAN                        HighLevel
  )
{
  UINT32                            RegValue;
  UINT32                            GpioBaseAddress;
  UINT32                            GpioNumMask;

  GpioBaseAddress =  LpcPciCfg32 (R_QNC_LPC_GBA_BASE) & B_QNC_LPC_GPA_BASE_MASK;
  ASSERT (GpioBaseAddress > 0);

  RegValue = IoRead32 (GpioBaseAddress + LevelRegOffset);
  GpioNumMask = (1 << GpioNum);
  if (HighLevel) {
    RegValue |= (GpioNumMask);
  } else {
    RegValue &= ~(GpioNumMask);
  }
  IoWrite32 (GpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL, RegValue);
}

//
// Routines exported by this component.
//

/**
  Platform assert PCI express PERST# signal.

  @param   PciExpPerstResumeWellGpio     PCIEXP_PERST_RESUMEWELL_GPIO number.

**/
VOID
EFIAPI
PlatformPERSTAssert (
  IN CONST UINT32              PciExpPerstResumeWellGpio
  )
{
  LegacyGpioSetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, PciExpPerstResumeWellGpio, FALSE);
}

/**
  Platform de assert PCI express PERST# signal.

  @param   PciExpPerstResumeWellGpio     PCIEXP_PERST_RESUMEWELL_GPIO number.

**/
VOID
EFIAPI
PlatformPERSTDeAssert (
  IN CONST UINT32              PciExpPerstResumeWellGpio
  )
{
  LegacyGpioSetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, PciExpPerstResumeWellGpio, TRUE);
}

/** Early initialisation of the PCIe controller.

  @retval   EFI_SUCCESS               Operation success.

**/
EFI_STATUS
EFIAPI
PlatformPciExpressEarlyInit (
  VOID
  )
{

  //
  // Release and wait for PCI controller to come out of reset.
  //
  SocUnitReleasePcieControllerPreWaitPllLock ();
  MicroSecondDelay (PCIEXP_DELAY_US_WAIT_PLL_LOCK);
  SocUnitReleasePcieControllerPostPllLock ();

  //
  // Early PCIe initialisation
  //
  SocUnitEarlyInitialisation ();

  //
  // Do North cluster early PCIe init.
  //
  PciExpressEarlyInit ();

  return EFI_SUCCESS;
}

