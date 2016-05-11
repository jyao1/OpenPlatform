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

#include <QuarkNcSocId.h>
#include <Ioh.h>
#include <Library/I2cLib.h>
#include <Library/PciLib.h>

#include <PlatformBoards.h>
#include <Galileo/Include/BoardGalileo.h>
#include <Library/PlatformHelperLib.h>

BOARD_LEGACY_GPIO_CONFIG      mBoardLegacyGpioConfigTable  = GALILEO_LEGACY_GPIO_INITIALIZER;
BOARD_GPIO_CONTROLLER_CONFIG  mBoardGpioControllerConfigTable  = GALILEO_GPIO_CONTROLLER_INITIALIZER;

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

/**
  Initialize state of I2C GPIO expanders.

**/
EFI_STATUS
EarlyPlatformConfigGpioExpanders (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_I2C_DEVICE_ADDRESS  I2CSlaveAddress;
  UINTN                   Length;
  UINTN                   ReadLength;
  UINT8                   Buffer[2];

  //
  // Configure GPIO expanders for Galileo
  // Set all GPIO expander pins connected to the Reset Button as inputs
  // Route I2C pins to Arduino header
  //

  //
  // Detect the I2C Slave Address of the GPIO Expander
  //
  if (PlatformLegacyGpioGetLevel(R_QNC_GPIO_RGLVL_RESUME_WELL, GALILEO_DETERMINE_IOEXP_SLA_RESUMEWELL_GPIO)) {
    I2CSlaveAddress.I2CDeviceAddress = GALILEO_IOEXP_J2HI_7BIT_SLAVE_ADDR;
  } else {
    I2CSlaveAddress.I2CDeviceAddress = GALILEO_IOEXP_J2LO_7BIT_SLAVE_ADDR;
  }
  DEBUG((EFI_D_INFO, "Galileo GPIO Expender Slave Address = %02x\n", I2CSlaveAddress.I2CDeviceAddress));

  //
  // Set I2C_MUX (GPORT1_BIT5) low to route I2C to Arduino Shield connector
  //

  //
  // Select GPIO Expander GPORT1
  //
  Length = 2;
  Buffer[0] = 0x18; //sub-address
  Buffer[1] = 0x01; //data
  Status = I2cWriteMultipleByte(
      I2CSlaveAddress,
      EfiI2CSevenBitAddrMode,
      &Length,
      &Buffer
      );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_WARN, "Fail to select GPIO Expander\n"));
    return EFI_SUCCESS;
  }

  //
  // Read "Pin Direction" of GPIO Expander GPORT1
  //
  Length = 1;
  ReadLength = 1;
  Buffer[1] = 0x1C;
  Status = I2cReadMultipleByte (
      I2CSlaveAddress,
      EfiI2CSevenBitAddrMode,
      &Length,
      &ReadLength,
      &Buffer[1]
      );
  ASSERT_EFI_ERROR (Status);

  //
  // Configure GPIO Expander GPORT1_BIT5 as an output
  //
  Length = 2;
  Buffer[0] = 0x1C; //sub-address
  Buffer[1] = (UINT8)(Buffer[1] & (~BIT5)); //data

  Status = I2cWriteMultipleByte (
      I2CSlaveAddress,
      EfiI2CSevenBitAddrMode,
      &Length,
      &Buffer
      );
  ASSERT_EFI_ERROR (Status);

  //
  // Set GPIO Expander GPORT1_BIT5 low
  //
  Length = 2;
  Buffer[0] = 0x09; //sub-address
  Buffer[1] = (UINT8)(~BIT5); //data

  Status = I2cWriteMultipleByte (
      I2CSlaveAddress,
      EfiI2CSevenBitAddrMode,
      &Length,
      &Buffer
      );
  ASSERT_EFI_ERROR (Status);

  //
  // Configure RESET_N_SHLD (GPORT5_BIT0) and SW_RESET_N_SHLD (GPORT5_BIT1) as inputs
  //

  //
  // Select GPIO Expander GPORT5
  //
  Length = 2;
  Buffer[0] = 0x18;
  Buffer[1] = 0x05;
  Status = I2cWriteMultipleByte (
      I2CSlaveAddress,
      EfiI2CSevenBitAddrMode,
      &Length,
      &Buffer
      );
  ASSERT_EFI_ERROR (Status);

  //
  // Read "Pin Direction" of GPIO Expander GPORT5
  //
  Length = 1;
  ReadLength = 1;
  Buffer[1] = 0x1C;
  Status = I2cReadMultipleByte (
      I2CSlaveAddress,
      EfiI2CSevenBitAddrMode,
      &Length,
      &ReadLength,
      &Buffer[1]
      );
  ASSERT_EFI_ERROR (Status);

  //
  // Configure GPIO Expander GPORT5_BIT0 and GPORT5_BIT1 as inputs
  //
  Length = 2;
  Buffer[0] = 0x1C;
  Buffer[1] = Buffer[1] | BIT0 | BIT1;
  Status = I2cWriteMultipleByte (
      I2CSlaveAddress,
      EfiI2CSevenBitAddrMode,
      &Length,
      &Buffer
      );
  ASSERT_EFI_ERROR (Status);

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
  UINT32                            IohGpioBase;
  UINT32                            Data32;
  UINT32                            Addr;
  UINT32                            DevPcieAddr;
  UINT16                            SaveCmdReg;
  UINT32                            SaveBarReg;
  UINT16                            PciVid;
  UINT16                            PciDid;

  IohGpioBase = (UINT32) PcdGet64 (PcdIohGpioMmioBase);

  DevPcieAddr = PCI_LIB_ADDRESS (
                  PcdGet8 (PcdIohGpioBusNumber),
                  PcdGet8 (PcdIohGpioDevNumber),
                  PcdGet8 (PcdIohGpioFunctionNumber),
                  0
                  );

  //
  // Do nothing if not a supported device.
  //
  PciVid = PciRead16 (DevPcieAddr + PCI_VENDOR_ID_OFFSET);
  PciDid = PciRead16 (DevPcieAddr + PCI_DEVICE_ID_OFFSET);
  if((PciVid != V_IOH_I2C_GPIO_VENDOR_ID) || (PciDid != V_IOH_I2C_GPIO_DEVICE_ID)) {
    return;
  }

  //
  // Save current settings for PCI CMD/BAR registers.
  //
  SaveCmdReg = PciRead16 (DevPcieAddr + PCI_COMMAND_OFFSET);
  SaveBarReg = PciRead32 (DevPcieAddr + PcdGet8 (PcdIohGpioBarRegister));

  //
  // Use predefined tempory memory resource.
  //
  PciWrite32 ( DevPcieAddr + PcdGet8 (PcdIohGpioBarRegister), IohGpioBase);
  PciWrite8 ( DevPcieAddr + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Gpio Controller Manipulation Tasks.
  //

    //
    // Reset Cypress Expander on Galileo Platform
    //
    Addr = IohGpioBase + GPIO_SWPORTA_DR;
    Data32 = *((volatile UINT32 *) (UINTN)(Addr));
    Data32 |= BIT4;                                 // Cypress Reset line controlled by GPIO<4>
    *((volatile UINT32 *) (UINTN)(Addr)) = Data32;

    Data32 = *((volatile UINT32 *) (UINTN)(Addr));
    Data32 &= ~BIT4;                                // Cypress Reset line controlled by GPIO<4>
    *((volatile UINT32 *) (UINTN)(Addr)) = Data32;

  //
  // Restore settings for PCI CMD/BAR registers
  //
  PciWrite32 ((DevPcieAddr + PcdGet8 (PcdIohGpioBarRegister)), SaveBarReg);
  PciWrite16 (DevPcieAddr + PCI_COMMAND_OFFSET, SaveCmdReg);
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
  
  DEBUG ((EFI_D_INFO, "Detecting Galileo ...\n"));
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
  if (PcdGet16(PcdPlatformType) != Galileo) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((EFI_D_INFO, "Detected Galileo!\n"));

  Status = PcdSet64S (PcdBoardInitPreMem, (UINT64)(UINTN)BoarInitPreMem);
  ASSERT_EFI_ERROR(Status);
  Status = PcdSet64S (PcdBoardInitPostMem, (UINT64)(UINTN)BoarInitPostMem);
  ASSERT_EFI_ERROR(Status);

  Status = PcdSet32S (PcdPciExpPerstResumeWellGpio, PCIEXP_PERST_RESUMEWELL_GPIO);
  ASSERT_EFI_ERROR(Status);
  Status = PcdSet32S (PcdFlashUpdateLedResumeWellGpio, GALILEO_FLASH_UPDATE_LED_RESUMEWELL_GPIO);
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
