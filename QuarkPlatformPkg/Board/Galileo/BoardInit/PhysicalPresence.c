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
#include <Library/I2cLib.h>

#include <PlatformBoards.h>
#include <Galileo/Include/BoardGalileo.h>

BOOLEAN
CheckResetButtonState (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_I2C_DEVICE_ADDRESS  I2CSlaveAddress;
  UINTN                   Length;
  UINTN                   ReadLength;
  UINT8                   Buffer[2];

  DEBUG ((EFI_D_INFO, "CheckResetButtonState()\n"));

  //
  // Detect the I2C Slave Address of the GPIO Expander
  //
  if (PlatformLegacyGpioGetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, GALILEO_DETERMINE_IOEXP_SLA_RESUMEWELL_GPIO)) {
    I2CSlaveAddress.I2CDeviceAddress = GALILEO_IOEXP_J2HI_7BIT_SLAVE_ADDR;
  } else {
    I2CSlaveAddress.I2CDeviceAddress = GALILEO_IOEXP_J2LO_7BIT_SLAVE_ADDR;
  }
  DEBUG ((EFI_D_INFO, "Galileo GPIO Expender Slave Address = %02x\n", I2CSlaveAddress.I2CDeviceAddress));

  //
  // Read state of RESET_N_SHLD (GPORT5_BIT0)
  //
  Buffer[1] = 5;
  Length = 1;
  ReadLength = 1;
  Status = I2cReadMultipleByte (
             I2CSlaveAddress,
             EfiI2CSevenBitAddrMode,
             &Length,
             &ReadLength,
             &Buffer[1]
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Return the state of GPORT5_BIT0
  //
  return ((Buffer[1] & BIT0) != 0);
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

  if (PcdGet16(PcdPlatformType) != Galileo) {
    return EFI_UNSUPPORTED;
  }
  //
  // Read the state of the reset button when the library is initialized
  //
  Status = PcdSetBoolS (PcdUserIsPhysicallyPresent, !CheckResetButtonState ());
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
