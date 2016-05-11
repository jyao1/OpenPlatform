/** @file
This PEIM initialize platform for MRC, following action is performed,
1. Initizluize GMCH
2. Detect boot mode
3. Detect video adapter to determine whether we need pre allocated memory
4. Calls MRC to initialize memory and install a PPI notify to do post memory initialization.
This file contains the main entrypoint of the PEIM.

Copyright (c) 2013 - 2016 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include "CommonHeader.h"
#include "PlatformEarlyInit.h"
#include "PeiFvSecurity.h"
#include <Ppi/BoardDetection.h>

static EFI_PEI_PPI_DESCRIPTOR mBoardDetectionStartPpi[] = {
  {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gBoardDetectionStartPpiGuid,
  NULL
  }
};

EFI_STATUS
EFIAPI
EndOfPeiSignalPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

//
// Function prototypes to routines implemented in other source modules
// within this component.
//

EFI_STATUS
EFIAPI
PlatformErratasPostMrc (
  VOID
  );

//
// The global indicator, the FvFileLoader callback will modify it to TRUE after loading PEIM into memory
//
BOOLEAN ImageInMemory = FALSE;

EFI_PEI_PPI_DESCRIPTOR mPpiBootMode[1] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMasterBootModePpiGuid,
    NULL
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR mMemoryDiscoveredNotifyList[1] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    MemoryDiscoveredPpiNotifyCallback
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR mEndOfPeiSignalPpiNotifyList[1] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEndOfPeiSignalPpiGuid,
    EndOfPeiSignalPpiNotifyCallback
  }
};

EFI_PEI_STALL_PPI mStallPpi = {
  PEI_STALL_RESOLUTION,
  Stall
};

EFI_PEI_PPI_DESCRIPTOR mPpiStall[1] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiStallPpiGuid,
    &mStallPpi
  }
};

/**
  This is the entrypoint of PEIM

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS if it completed successfully.
**/
EFI_STATUS
EFIAPI
PeiInitPlatform (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                              Status;
  EFI_BOOT_MODE                           BootMode;
  EFI_PEI_STALL_PPI                       *StallPpi;
  EFI_PEI_PPI_DESCRIPTOR                  *StallPeiPpiDescriptor;
  EFI_FV_FILE_INFO                        FileInfo;
  BOARD_INIT                              BoardInit;
  VOID                                    *Instance;

  //
  // Start board detection
  //
  Status = PeiServicesInstallPpi(mBoardDetectionStartPpi);
  ASSERT_EFI_ERROR(Status);
  
  //
  // Check if board detection is finished.
  //
  Status = PeiServicesLocatePpi (
             &gBoardDetectedPpiGuid,
             0,
             NULL,
             &Instance
             );
  ASSERT_EFI_ERROR(Status);

  //
  // Initialize Firmware Volume security.
  // This must be done before any firmware volume accesses (excl. BFV)
  //
  Status = PeiInitializeFvSecurity();
  ASSERT_EFI_ERROR (Status);

  //
  // Do any early board specific initialization.
  //
  BoardInit = (BOARD_INIT)(UINTN)PcdGet64(PcdBoardInitPreMem);
  if (BoardInit != NULL) {
    BoardInit ();
  }

  //
  // This is a second path on entry, in recovery boot path the Stall PPI need to be memory-based
  // to improve recovery performance.
  //
  Status = PeiServicesFfsGetFileInfo (FileHandle, &FileInfo);
  ASSERT_EFI_ERROR (Status);
  //
  // The follow conditional check only works for memory-mapped FFS,
  // so we ASSERT that the file is really a MM FFS.
  //
  ASSERT (FileInfo.Buffer != NULL);
  if (!(((UINTN) FileInfo.Buffer <= (UINTN) PeiInitPlatform) &&
        ((UINTN) PeiInitPlatform <= (UINTN) FileInfo.Buffer + FileInfo.BufferSize))) {
    //
    // Now that module in memory, update the
    // PPI that describes the Stall to other modules
    //
    Status = PeiServicesLocatePpi (
               &gEfiPeiStallPpiGuid,
               0,
               &StallPeiPpiDescriptor,
               (VOID **) &StallPpi
               );

    if (!EFI_ERROR (Status)) {

      Status = PeiServicesReInstallPpi (
                 StallPeiPpiDescriptor,
                 &mPpiStall[0]
                 );
    } else {

      Status = PeiServicesInstallPpi (&mPpiStall[0]);
    }
    return Status;
  }

  //
  // Initialize System Phys
  //

  // Program USB Phy
  InitializeUSBPhy();

  //
  // Do platform specific logic to create a boot mode
  //
  Status = UpdateBootMode ((EFI_PEI_SERVICES**)PeiServices, &BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Signal possible dependent modules that there has been a
  // final boot mode determination
  //
  if (!EFI_ERROR(Status)) {
    Status = PeiServicesInstallPpi (&mPpiBootMode[0]);
    ASSERT_EFI_ERROR (Status);
  }

  if (BootMode != BOOT_ON_S3_RESUME) {
    QNCClearSmiAndWake ();
  }

  DEBUG ((EFI_D_INFO, "MRC Entry\n"));
  MemoryInit ((EFI_PEI_SERVICES**)PeiServices);

  //
  // Do Early PCIe init.
  //
  DEBUG ((EFI_D_INFO, "Early PCIe controller initialization\n"));
  PlatformPciExpressEarlyInit ();


  DEBUG ((EFI_D_INFO, "Platform Erratas After MRC\n"));
  PlatformErratasPostMrc ();

  //
  //
  //
  BoardInit = (BOARD_INIT)(UINTN)PcdGet64(PcdBoardInitPostMem);
  if (BoardInit != NULL) {
    BoardInit ();
  }

  //
  // Now that all of the pre-permanent memory activities have
  // been taken care of, post a call-back for the permanent-memory
  // resident services, such as HOB construction.
  // PEI Core will switch stack after this PEIM exit.  After that the MTRR
  // can be set.
  //
  Status = PeiServicesNotifyPpi (&mMemoryDiscoveredNotifyList[0]);
  ASSERT_EFI_ERROR (Status);
/*

  if (BootMode != BOOT_ON_S3_RESUME) {
    Status = PeiServicesNotifyPpi (mEndOfPeiSignalPpiNotifyList);
    ASSERT_EFI_ERROR (Status);
  }
*/
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    PeiServicesRegisterForShadow (FileHandle);
  }

  return Status;
}

EFI_STATUS
EFIAPI
EndOfPeiSignalPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                            Status;

  DEBUG ((EFI_D_INFO, "End of PEI Signal Callback\n"));

    //
  // Restore the flash region to be UC
  // for both normal boot as we build a Resource Hob to
  // describe this region as UC to DXE core.
  //
  WriteBackInvalidateDataCacheRange (
    (VOID *) (UINTN) PcdGet32 (PcdFlashAreaBaseAddress),
    PcdGet32 (PcdFlashAreaSize)
  );

  Status = MtrrSetMemoryAttribute (PcdGet32 (PcdFlashAreaBaseAddress), PcdGet32 (PcdFlashAreaSize), CacheUncacheable);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
