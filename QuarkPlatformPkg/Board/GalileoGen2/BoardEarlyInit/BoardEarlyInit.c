/** @file

Copyright (c) 2013 - 2016 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PeiServicesLib.h>
#include <IndustryStandard/Pci22.h>
#include <Ppi/BoardDetection.h>


#include <PlatformBoards.h>
#include <GalileoGen2/Include/BoardGalileoGen2.h>
#include <Library/PlatformHelperLib.h>
#include <GalileoGen2/Include/Library/Pcal9555Lib.h>

BOARD_LEGACY_GPIO_CONFIG      mBoardLegacyGpioConfigTable  = GALILEO_GEN2_LEGACY_GPIO_INITIALIZER;
BOARD_GPIO_CONTROLLER_CONFIG  mBoardGpioControllerConfigTable  = GALILEO_GEN2_GPIO_CONTROLLER_INITIALIZER;

/**
  This function provides early platform initialisation.

  @param  PlatformInfo  Pointer to platform Info structure.

**/
VOID
EFIAPI
EarlyPlatformInit (
  VOID
  );

/**
  Performs any early platform specific Legacy GPIO manipulation.

**/
VOID
EFIAPI
EarlyPlatformLegacyGpioManipulation (
  VOID
  );

/**
  Performs any early platform specific GPIO Controller manipulation.

**/
VOID
EFIAPI
EarlyPlatformGpioCtrlerManipulation (
  VOID
  );

EFI_STATUS
EFIAPI
PlatformSecureLibInitialize (
  VOID
  );

/**
  Initialize state of I2C GPIO expanders.

**/
EFI_STATUS
EarlyPlatformConfigGpioExpanders (
  VOID
  )
{

  //
  // Configure GPIO expanders for Galileo Gen 2
  // Route I2C pins to Arduino header
  // Set all GPIO expander pins connected to the Reset Button as inputs
  //

  //
  // Configure AMUX1_IN (EXP2.P1_4) as an output
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP2_7BIT_SLAVE_ADDR,  // IO Expander 2.
    12,                                   // P1-4.
    FALSE                                 // Configure as output
    );

  //
  // Set AMUX1_IN(EXP2.P1_4) low to route I2C to Arduino Shield connector
  //
  PlatformPcal9555GpioSetLevel (
    GALILEO_GEN2_IOEXP2_7BIT_SLAVE_ADDR,  // IO Expander 2.
    12,                                   // P1-4. 
    FALSE                                 // Set pin low
    );

  //
  // Configure Reset Button(EXP1.P1_7) as an input
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  // IO Expander 1.
    15,                                   // P1-7.
    TRUE
    );

  //
  // Disable pullup on Reset Button(EXP1.P1_7)
  //
  PlatformPcal9555GpioDisablePull (
    GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  // IO Expander 1.
    15                                    // P1-7.
    );

  //
  // Configure Reset Button(EXP2.P1_7) as an input
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP2_7BIT_SLAVE_ADDR,  // IO Expander 2.
    15,                                   // P1-7.
    TRUE
    );

  //
  // Disable pullup on Reset Button(EXP2.P1_7)
  //
  PlatformPcal9555GpioDisablePull (
    GALILEO_GEN2_IOEXP2_7BIT_SLAVE_ADDR,  // IO Expander 2.
    15                                    // P1-7.
    );

  return EFI_SUCCESS;
}

VOID
EFIAPI
BoarInitPreMem (
  VOID
  )
{

  //
  // Do any early platform specific initialization.
  //
  EarlyPlatformInit ();
}

VOID
EFIAPI
BoarInitPostMem (
  VOID
  )
{
  //
  //
  //
  DEBUG ((EFI_D_INFO, "EarlyPlatformConfigGpioExpanders ()\n"));
  EarlyPlatformConfigGpioExpanders ();

  PlatformSecureLibInitialize();
}

/**
  This function provides early platform initialization.

  @param  PlatformInfo  Pointer to platform Info structure.

**/
VOID
EFIAPI
EarlyPlatformInit (
  VOID
  )
{

  DEBUG ((EFI_D_INFO, "EarlyBoardInit for PlatType=0x%02x\n", PcdGet16 (PcdPlatformType)));

  //
  // Check if system reset due to error condition.
  //
  if (CheckForResetDueToErrors (TRUE)) {
    if(FeaturePcdGet (WaitIfResetDueToError)) {
      DEBUG ((EFI_D_ERROR, "Press any key to continue.\n"));
      PlatformDebugPortGetChar8 ();
    }
  }

  //
  // Display platform info messages.
  //
  EarlyPlatformInfoMessages ();

  //
  // Early Legacy Gpio Init.
  //
  EarlyPlatformLegacyGpioInit (&mBoardLegacyGpioConfigTable);

  //
  // Early platform Legacy GPIO manipulation depending on GPIOs
  // setup by EarlyPlatformLegacyGpioInit.
  //
  EarlyPlatformLegacyGpioManipulation ();

  //
  // Early platform specific GPIO Controller init & manipulation.
  // Combined for sharing of temp. memory bar.
  //
  EarlyPlatformGpioCtrlerInit (&mBoardGpioControllerConfigTable);
  
  EarlyPlatformGpioCtrlerManipulation ();

  //
  // Early Thermal Sensor Init.
  //
  EarlyPlatformThermalSensorInit ();

  //
  // Early Lan Ethernet Mac Init.
  //
  EarlyPlatformMacInit (
    PcdGetPtr (PcdIohEthernetMac0),
    PcdGetPtr (PcdIohEthernetMac1)
    );
}

/**
  Performs any early platform specific Legacy GPIO manipulation.

**/
VOID
EFIAPI
EarlyPlatformLegacyGpioManipulation (
  VOID
  )
{

}

/**
  Performs any early platform specific GPIO Controller manipulation.

**/
VOID
EFIAPI
EarlyPlatformGpioCtrlerManipulation (
  VOID
  )
{

}

static EFI_PEI_PPI_DESCRIPTOR mDetectedPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardDetectedPpiGuid,
  NULL
};

EFI_STATUS
EFIAPI
BoardDetectionCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS   Status;
  VOID         *Instance;
  
  DEBUG ((EFI_D_INFO, "Detecting GalileoGen2 ...\n"));
  //
  // Check if board detection is finished.
  //
  Status = PeiServicesLocatePpi (
             &gBoardDetectedPpiGuid,
             0,
             NULL,
             &Instance
             );
  if (!EFI_ERROR(Status)) {
    return EFI_SUCCESS;
  }

  //
  // In most real platform, here should be the code to detect board type.
  // 
  // For galileo, we only support build time board selection, so use PCD to do the fake detection.
  //
  if (PcdGet16(PcdPlatformType) != GalileoGen2) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((EFI_D_INFO, "Detected GalileoGen2!\n"));

  Status = PcdSet64S (PcdBoardInitPreMem, (UINT64)(UINTN)BoarInitPreMem);
  ASSERT_EFI_ERROR(Status);
  Status = PcdSet64S (PcdBoardInitPostMem, (UINT64)(UINTN)BoarInitPostMem);
  ASSERT_EFI_ERROR(Status);

  Status = PcdSet32S(PcdPciExpPerstResumeWellGpio, GALILEO_GEN2_PCIEXP_PERST_RESUMEWELL_GPIO);
  ASSERT_EFI_ERROR(Status);
  Status = PcdSet32S(PcdFlashUpdateLedResumeWellGpio, GALILEO_GEN2_FLASH_UPDATE_LED_RESUMEWELL_GPIO);
  ASSERT_EFI_ERROR(Status);

  Status = PeiServicesInstallPpi(&mDetectedPpi);
  ASSERT_EFI_ERROR(Status);
  return Status;
}

static EFI_PEI_NOTIFY_DESCRIPTOR mDetectionStartNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardDetectionStartPpiGuid,
  BoardDetectionCallback
};

/**
  This is the entrypoint of PEIM

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS if it completed successfully.
**/
EFI_STATUS
EFIAPI
PeiInitBoard (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;
  //
  // Register board detection callback function.
  //
  Status = PeiServicesNotifyPpi(&mDetectionStartNotifyList);

  return Status;
}
