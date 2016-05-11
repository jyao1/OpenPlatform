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

#include <QuarkNcSocId.h>
#include <Library/I2cLib.h>

#include <PlatformBoards.h>
#include <Galileo/Include/BoardGalileo.h>
#include <Protocol/PlatformSmmSpiReady.h>
#include <Library/PlatformHelperLib.h>

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
PlatformInitializeUart0MuxGalileo (
  VOID
  )
/*++


Routine Description:

  This is the routine to initialize UART0 for DBG2 support. The hardware used in this process is a
  Legacy Bridge (Legacy GPIO), I2C controller, a bi-directional MUX and a Cypress CY8C9540A chip.

Arguments:

  None.

Returns:

  None.

--*/
{
  EFI_STATUS                        Status;
  EFI_I2C_DEVICE_ADDRESS            I2CSlaveAddress;
  UINTN                             Length;
  UINT8                             Buffer[2];

  if (PlatformLegacyGpioGetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, GALILEO_DETERMINE_IOEXP_SLA_RESUMEWELL_GPIO)) {
    I2CSlaveAddress.I2CDeviceAddress = GALILEO_IOEXP_J2HI_7BIT_SLAVE_ADDR;
  } else {
    I2CSlaveAddress.I2CDeviceAddress = GALILEO_IOEXP_J2LO_7BIT_SLAVE_ADDR;
  }

  //
  // Set GPIO_SUS<2> as an output, raise voltage to Vdd.
  //
  PlatformLegacyGpioSetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, 2, TRUE);

  //
  // Select Port 3
  //
  Length = 2;
  Buffer[0] = 0x18; //sub-address
  Buffer[1] = 0x03; //data

  Status = I2cWriteMultipleByte (
              I2CSlaveAddress,
              EfiI2CSevenBitAddrMode,
              &Length,
              &Buffer
              );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN, "Fail to select GPIO Port 3\n"));
    return ;
  }

  //
  // Set "Pin Direction" bit4 and bit5 as outputs
  //
  Length = 2;
  Buffer[0] = 0x1C; //sub-address
  Buffer[1] = 0xCF; //data

  Status = I2cWriteMultipleByte (
              I2CSlaveAddress,
              EfiI2CSevenBitAddrMode,
              &Length,
              &Buffer
              );
  ASSERT_EFI_ERROR (Status);

  //
  // Lower GPORT3 bit4 and bit5 to Vss
  //
  Length = 2;
  Buffer[0] = 0x0B; //sub-address
  Buffer[1] = 0xCF; //data

  Status = I2cWriteMultipleByte (
              I2CSlaveAddress,
              EfiI2CSevenBitAddrMode,
              &Length,
              &Buffer
              );
  ASSERT_EFI_ERROR (Status);
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
    // Use MUX to connect out UART0 pins.
    //
    PlatformInitializeUart0MuxGalileo ();

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

EFI_STATUS
EFIAPI
BoardInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{

  if (PcdGet16 (PcdPlatformType) != Galileo) {
    return EFI_UNSUPPORTED;
  }

  //
  // Create events for configuration callbacks.
  //
  CreateConfigEvents ();

  return EFI_SUCCESS;
}
