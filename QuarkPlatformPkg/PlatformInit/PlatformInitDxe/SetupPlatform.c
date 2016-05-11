/** @file
Platform Initialization Driver.

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"

#include "SetupPlatform.h"
#include <Library/HobLib.h>

EFI_HANDLE            mImageHandle = NULL;

UINT8                    mSmbusRsvdAddresses[PLATFORM_NUM_SMBUS_RSVD_ADDRESSES] = {
  SMBUS_ADDR_CH_A_1,
  SMBUS_ADDR_CK505,
  SMBUS_ADDR_THERMAL_SENSOR1,
  SMBUS_ADDR_THERMAL_SENSOR2
};

EFI_PLATFORM_POLICY_PROTOCOL    mPlatformPolicyData = {
  PLATFORM_NUM_SMBUS_RSVD_ADDRESSES,
  mSmbusRsvdAddresses
};

EFI_STATUS
EFIAPI
DxePlatformDriverEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++

  Routine Description:
    This is the standard EFI driver point for the Platform Driver. This
    driver is responsible for setting up any platform specific policy or
    initialization information.

  Arguments:
    ImageHandle     - Handle for the image of this driver
    SystemTable     - Pointer to the EFI System Table

  Returns:
    EFI_SUCCESS     - Policy decisions set

--*/
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;

  S3BootScriptSaveInformationAsciiString (
    "SetupDxeEntryBegin"
    );

  mImageHandle = ImageHandle;

  //
  // Initialize ICH registers
  //
  PlatformInitQNCRegs();

  //
  // Install protocol to to allow access to this Policy.
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiPlatformPolicyProtocolGuid, &mPlatformPolicyData,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  S3BootScriptSaveInformationAsciiString (
    "SetupDxeEntryEnd"
    );

  return EFI_SUCCESS;
}

