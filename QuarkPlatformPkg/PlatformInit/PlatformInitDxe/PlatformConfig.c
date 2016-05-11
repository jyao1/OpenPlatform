/** @file
Essential platform configuration.

Copyright (c) 2013 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/

#include "PlatformInitDxe.h"

//
// The protocols, PPI and GUID defintions for this module
//

//
// The Library classes this module consumes
//

//
// RTC:28208 - System hang/crash when entering probe mode(ITP) when relocating SMBASE
//             Workaround to make default SMRAM UnCachable
//
#define SMM_DEFAULT_SMBASE                  0x30000     // Default SMBASE address
#define SMM_DEFAULT_SMBASE_SIZE_BYTES       0x10000     // Size in bytes of default SMRAM

BOOLEAN                       mMemCfgDone = FALSE;

VOID
EFIAPI
PlatformConfigOnSmmConfigurationProtocol (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
/*++

Routine Description:

  Function runs in PI-DXE to perform platform specific config when
  SmmConfigurationProtocol is installed.

Arguments:
  Event       - The event that occured.
  Context     - For EFI compatiblity.  Not used.

Returns:
  None.
--*/

{
  EFI_STATUS            Status;
  UINT32                NewValue;
  UINT64                BaseAddress;
  UINT64                SmramLength;
  VOID                  *SmmCfgProt;

  Status = gBS->LocateProtocol (&gEfiSmmConfigurationProtocolGuid, NULL, &SmmCfgProt);
  if (Status != EFI_SUCCESS){
    DEBUG ((DEBUG_INFO, "gEfiSmmConfigurationProtocolGuid triggered but not valid.\n"));
    return;
  }
  if (mMemCfgDone) {
    DEBUG ((DEBUG_INFO, "Platform DXE Mem config already done.\n"));
    return;
  }

  //
  // Disable eSram block (this will also clear/zero eSRAM)
  // We only use eSRAM in the PEI phase. Disable now that we are in the DXE phase
  //
  NewValue = QNCPortRead (QUARK_NC_MEMORY_MANAGER_SB_PORT_ID, QUARK_NC_MEMORY_MANAGER_ESRAMPGCTRL_BLOCK);
  NewValue |= BLOCK_DISABLE_PG;
  QNCPortWrite (QUARK_NC_MEMORY_MANAGER_SB_PORT_ID, QUARK_NC_MEMORY_MANAGER_ESRAMPGCTRL_BLOCK, NewValue);

  //
  // Update HMBOUND to top of DDR3 memory and LOCK
  // We disabled eSRAM so now we move HMBOUND down to top of DDR3
  //
  QNCGetTSEGMemoryRange (&BaseAddress, &SmramLength);
  NewValue = (UINT32)(BaseAddress + SmramLength);
  DEBUG ((EFI_D_INFO,"Locking HMBOUND at: = 0x%8x\n",NewValue));
  QNCPortWrite (QUARK_NC_HOST_BRIDGE_SB_PORT_ID, QUARK_NC_HOST_BRIDGE_HMBOUND_REG, (NewValue | HMBOUND_LOCK));

  //
  // Lock IMR5 now that HMBOUND is locked (legacy S3 region)
  //
  NewValue = QNCPortRead (QUARK_NC_MEMORY_MANAGER_SB_PORT_ID, QUARK_NC_MEMORY_MANAGER_IMR5+QUARK_NC_MEMORY_MANAGER_IMRXL);
  NewValue |= IMR_LOCK;
  QNCPortWrite (QUARK_NC_MEMORY_MANAGER_SB_PORT_ID, QUARK_NC_MEMORY_MANAGER_IMR5+QUARK_NC_MEMORY_MANAGER_IMRXL, NewValue);

  //
  // Lock IMR6 now that HMBOUND is locked (ACPI Reclaim/ACPI/Runtime services/Reserved)
  //
  NewValue = QNCPortRead (QUARK_NC_MEMORY_MANAGER_SB_PORT_ID, QUARK_NC_MEMORY_MANAGER_IMR6+QUARK_NC_MEMORY_MANAGER_IMRXL);
  NewValue |= IMR_LOCK;
  QNCPortWrite (QUARK_NC_MEMORY_MANAGER_SB_PORT_ID, QUARK_NC_MEMORY_MANAGER_IMR6+QUARK_NC_MEMORY_MANAGER_IMRXL, NewValue);

  //
  // Disable IMR2 memory protection (RMU Main Binary)
  //
  QncImrWrite (
            QUARK_NC_MEMORY_MANAGER_IMR2,
            (UINT32)(IMRL_RESET & ~IMR_EN),
            (UINT32)IMRH_RESET,
            (UINT32)IMRX_ALL_ACCESS,
            (UINT32)IMRX_ALL_ACCESS
        );

  //
  // Disable IMR3 memory protection (Default SMRAM)
  //
  QncImrWrite (
            QUARK_NC_MEMORY_MANAGER_IMR3,
            (UINT32)(IMRL_RESET & ~IMR_EN),
            (UINT32)IMRH_RESET,
            (UINT32)IMRX_ALL_ACCESS,
            (UINT32)IMRX_ALL_ACCESS
        );

  //
  // Disable IMR4 memory protection (eSRAM).
  //
  QncImrWrite (
            QUARK_NC_MEMORY_MANAGER_IMR4,
            (UINT32)(IMRL_RESET & ~IMR_EN),
            (UINT32)IMRH_RESET,
            (UINT32)IMRX_ALL_ACCESS,
            (UINT32)IMRX_ALL_ACCESS
        );

  //
  // RTC:28208 - System hang/crash when entering probe mode(ITP) when relocating SMBASE
  //             Workaround to make default SMRAM UnCachable
  //
  Status = gDS->SetMemorySpaceAttributes (
                  (EFI_PHYSICAL_ADDRESS) SMM_DEFAULT_SMBASE,
                  SMM_DEFAULT_SMBASE_SIZE_BYTES,
                  EFI_MEMORY_WB
                  );
  ASSERT_EFI_ERROR (Status);

  mMemCfgDone = TRUE;
}

VOID
EFIAPI
PlatformConfigOnSpiReady (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
/*++

Routine Description:

  Function runs in PI-DXE to perform platform specific config when SPI
  interface is ready.

Arguments:
  Event       - The event that occured.
  Context     - For EFI compatiblity.  Not used.

Returns:
  None.

--*/
{
  EFI_STATUS                        Status;
  VOID                              *SpiReadyProt = NULL;
  EFI_BOOT_MODE                      BootMode;

  BootMode = GetBootModeHob ();

  Status = gBS->LocateProtocol (&gEfiSmmSpiReadyProtocolGuid, NULL, &SpiReadyProt);
  if (Status != EFI_SUCCESS){
    DEBUG ((DEBUG_INFO, "gEfiSmmSpiReadyProtocolGuid triggered but not valid.\n"));
    return;
  }

  //
  // Lock regions SPI flash.
  //
  PlatformFlashLockPolicy (FALSE);

  //
  // Configurations and checks to be done when DXE tracing available.
  //

  //
  // Platform specific Signal routing.
  //
}

EFI_STATUS
EFIAPI
CreateConfigEvents (
  VOID
  )
/*++

Routine Description:

Arguments:
  None

Returns:
  EFI_STATUS

--*/
{
  EFI_EVENT   EventSmmCfg;
  EFI_EVENT   EventSpiReady;
  VOID        *RegistrationSmmCfg;
  VOID        *RegistrationSpiReady;

  //
  // Schedule callback for when SmmConfigurationProtocol installed.
  //
  EventSmmCfg = EfiCreateProtocolNotifyEvent (
                  &gEfiSmmConfigurationProtocolGuid,
                  TPL_CALLBACK,
                  PlatformConfigOnSmmConfigurationProtocol,
                  NULL,
                  &RegistrationSmmCfg
                  );
  ASSERT (EventSmmCfg != NULL);

  //
  // Schedule callback to setup SPI Flash Policy when SPI interface ready.
  //
  EventSpiReady = EfiCreateProtocolNotifyEvent (
                    &gEfiSmmSpiReadyProtocolGuid,
                    TPL_CALLBACK,
                    PlatformConfigOnSpiReady,
                    NULL,
                    &RegistrationSpiReady
                    );
  ASSERT (EventSpiReady != NULL);
  return EFI_SUCCESS;
}
