/** @file
Provides a secure platform-specific method to detect physically present user.

Copyright (c) 2013 - 2016 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Library/PlatformHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <QNCAccess.h>

#include <PlatformBoards.h>
#include <GalileoGen2/Include/BoardGalileoGen2.h>
#include <GalileoGen2/Include/Library/Pcal9555Lib.h>

BOOLEAN
CheckResetButtonState (
  VOID
  )
{
  DEBUG ((EFI_D_INFO, "CheckResetButtonState()\n"));

  //
  // Read state of Reset Button - EXP2.P1_7
  // This GPIO is pulled high when the button is not pressed
  // This GPIO reads low when button is pressed
  //
  return PlatformPcal9555GpioGetState (
      GALILEO_GEN2_IOEXP2_7BIT_SLAVE_ADDR,  // IO Expander 2.
      15                                    // P1-7.
      );
}

/**
  Determines if a user is physically present by reading the reset button state.

  @param  ImageHandle  The image handle of this driver.
  @param  SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS   Install the Secure Boot Helper Protocol successfully.

**/
EFI_STATUS
EFIAPI
PlatformSecureLibInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  if (PcdGet16(PcdPlatformType) != GalileoGen2) {
    return EFI_UNSUPPORTED;
  }

  //
  // Read the state of the reset button when the library is initialized
  //
  Status = PcdSetBoolS (PcdUserIsPhysicallyPresent, !CheckResetButtonState ());
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
