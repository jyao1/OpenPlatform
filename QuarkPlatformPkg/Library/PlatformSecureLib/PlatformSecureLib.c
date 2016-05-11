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
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

BOOLEAN  mUserIsPhysicallyPresent;

/**

  This function provides a platform-specific method to detect whether the platform
  is operating by a physically present user.

  Programmatic changing of platform security policy (such as disable Secure Boot,
  or switch between Standard/Custom Secure Boot mode) MUST NOT be possible during
  Boot Services or after exiting EFI Boot Services. Only a physically present user
  is allowed to perform these operations.

  NOTE THAT: This function cannot depend on any EFI Variable Service since they are
  not available when this function is called in AuthenticateVariable driver.

  @retval  TRUE       The platform is operated by a physically present user.
  @retval  FALSE      The platform is NOT operated by a physically present user.

**/
BOOLEAN
EFIAPI
UserPhysicalPresent (
  VOID
  )
{
  return mUserIsPhysicallyPresent;
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
  //
  // Read the state of the reset button when the library is initialized
  //
  mUserIsPhysicallyPresent = PcdGetBool (PcdUserIsPhysicallyPresent);
  DEBUG((EFI_D_INFO, "UserIsPhysicallyPresent - 0x%x\n", mUserIsPhysicallyPresent));

  return EFI_SUCCESS;
}
