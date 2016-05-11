/** @file
Platform helper LED routines.

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>

#include "CommonHeader.h"

//
// Routines defined in other source modules of this component.
//

//
// Routines local to this source module.
//

//
// Routines exported by this source module.
//

/**
  Init platform LEDs into known state.

  @retval  EFI_SUCCESS      Operation success.

**/
EFI_STATUS
EFIAPI
PlatformLedInit (
  VOID
  )
{
  EFI_BOOT_MODE             BootMode;
  UINT32                    FlashUpdateLedResumeWellGpio;

  BootMode = GetBootModeHob ();

  //
  // Init Flash update / recovery LED in OFF state.
  //
  if (BootMode == BOOT_ON_FLASH_UPDATE || BootMode == BOOT_IN_RECOVERY_MODE) {
    FlashUpdateLedResumeWellGpio = PcdGet32(PcdFlashUpdateLedResumeWellGpio);
    if (FlashUpdateLedResumeWellGpio != 0xFFFFFFFF) {
      PlatformLegacyGpioSetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, FlashUpdateLedResumeWellGpio, FALSE);
    }
  }

  return EFI_SUCCESS;
}

/**
  Turn on or off platform flash update LED.

  @param   TurnOn           If TRUE turn on else turn off.

  @retval  EFI_SUCCESS      Operation success.

**/
EFI_STATUS
EFIAPI
PlatformFlashUpdateLed (
  IN CONST BOOLEAN                        TurnOn
  )
{
  UINT32  FlashUpdateLedResumeWellGpio;

  FlashUpdateLedResumeWellGpio = PcdGet32(PcdFlashUpdateLedResumeWellGpio);
  if (FlashUpdateLedResumeWellGpio != 0xFFFFFFFF) {
    PlatformLegacyGpioSetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, FlashUpdateLedResumeWellGpio, TurnOn);
  }

  return EFI_SUCCESS;
}
