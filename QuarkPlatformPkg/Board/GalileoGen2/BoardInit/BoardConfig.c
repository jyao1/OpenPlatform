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

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>

#include <PlatformBoards.h>
#include <GalileoGen2/Include/BoardGalileoGen2.h>
#include <Protocol/PlatformSmmSpiReady.h>
#include <Library/PlatformHelperLib.h>
#include <GalileoGen2/Include/Library/Pcal9555Lib.h>

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

VOID
EFIAPI
PlatformInitializeUart0MuxGalileoGen2 (
  VOID
  )
/*++


Routine Description:

  This is the routine to initialize UART0 on GalileoGen2. The hardware used in this process is
  I2C controller and the configuring the following IO Expander signal.

  EXP1.P1_5 should be configured as an output & driven high.
  EXP1.P0_0 should be configured as an output & driven high.
  EXP0.P1_4 should be configured as an output, driven low.
  EXP1.P0_1 pullup should be disabled.
  EXP0.P1_5 Pullup should be disabled.

Arguments:

  None.

Returns:

  None.

--*/

{
  //
  //  EXP1.P1_5 should be configured as an output & driven high.
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  // IO Expander 1.
    13,                                   // P1-5.
    TRUE
    );
  PlatformPcal9555GpioSetLevel (
    GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  // IO Expander 1.
    13,                                   // P1-5.
    TRUE
    );

  //
  // EXP1.P0_0 should be configured as an output & driven high.
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  // IO Expander 0.
    0,                                    // P0_0.
    TRUE
    );
  PlatformPcal9555GpioSetLevel (
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  // IO Expander 0.
    0,                                    // P0_0.
    TRUE
    );

  //
  //  EXP0.P1_4 should be configured as an output, driven low.
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  // IO Expander 0.
    12,                                   // P1-4.
    FALSE
    );
  PlatformPcal9555GpioSetLevel (          // IO Expander 0.
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  // P1-4
    12,
    FALSE
    );

  //
  // EXP1.P0_1 pullup should be disabled.
  //
  PlatformPcal9555GpioDisablePull (
    GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  // IO Expander 1.
    1                                     // P0-1.
    );

  //
  // EXP0.P1_5 Pullup should be disabled.
  //
  PlatformPcal9555GpioDisablePull (
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  // IO Expander 0.
    13                                    // P1-5.
    );
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
  // Configurations and checks to be done when DXE tracing available.
  //

  //
  // Platform specific Signal routing.
  //

  //
  // Skip any signal not needed for recovery and flash update.
  //
  if (BootMode != BOOT_ON_FLASH_UPDATE && BootMode != BOOT_IN_RECOVERY_MODE) {
    //
    // Use route out UART0 pins.
    //
    PlatformInitializeUart0MuxGalileoGen2 ();
  }
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
  EFI_EVENT   EventSpiReady;
  VOID        *RegistrationSpiReady;

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

VOID
GalileoGen2RouteOutFlashUpdateLed (
  VOID
  )
{
  //
  // For GpioNums below values 0 to 7 are for Port0 ie P0-0 - P0-7 and
  // values 8 to 15 are for Port1 ie P1-0 - P1-7.
  //

  //
  // Disable Pull-ups / pull downs on EXP0 pin for LVL_B_PU7 signal.
  //
  PlatformPcal9555GpioDisablePull (
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  // IO Expander 0.
    15                                   // P1-7.
    );

  //
  // Make LVL_B_OE7_N an output pin.
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,  // IO Expander 0.
    14,                                   // P1-6.
    FALSE
    );

  //
  // Set level of LVL_B_OE7_N to low.
  //
  PlatformPcal9555GpioSetLevel (
    GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR,
    14,
    FALSE
    );

  //
  // Make MUX8_SEL an output pin.
  //
  PlatformPcal9555GpioSetDir (
    GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  // IO Expander 1.
    14,                                   // P1-6.
    FALSE
    );

  //
  // Set level of MUX8_SEL to low to route GPIO_SUS<5> to LED.
  //
  PlatformPcal9555GpioSetLevel (
    GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR,  // IO Expander 1.
    14,                                   // P1-6.
    FALSE
    );
}

EFI_STATUS
EFIAPI
BoardInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_BOOT_MODE             BootMode;

  if (PcdGet16 (PcdPlatformType) != GalileoGen2) {
    return EFI_UNSUPPORTED;
  }

  //
  // Create events for configuration callbacks.
  //
  CreateConfigEvents ();

  BootMode = GetBootModeHob();
  if (BootMode == BOOT_ON_FLASH_UPDATE || BootMode == BOOT_IN_RECOVERY_MODE) {
    GalileoGen2RouteOutFlashUpdateLed ();
  }

  return EFI_SUCCESS;
}
