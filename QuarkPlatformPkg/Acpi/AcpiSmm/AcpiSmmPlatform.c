/** @file
ACPISMM Driver implementation file.

This is QNC Smm platform driver

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/

#include <AcpiSmmPlatform.h>

//
// Modular variables needed by this driver
//
EFI_ACPI_SMM_DEV                 mAcpiSmm;

EFI_STATUS
EFIAPI
InitAcpiSmmPlatform (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
/*++

Routine Description:

  Initializes the SMM S3 Handler Driver.

Arguments:

  ImageHandle  -  The image handle of Sleep State Wake driver.
  SystemTable  -  The starndard EFI system table.

Returns:

  EFI_OUT_OF_RESOURCES  -  Insufficient resources to complete function.
  EFI_SUCCESS           -  Function has completed successfully.
  Other                 -  Error occured during execution.

--*/
{
  EFI_STATUS                      Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL    *AcpiNvsProtocol = NULL;

  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &AcpiNvsProtocol
                  );
  ASSERT_EFI_ERROR (Status);


  //
  // Locate and Register to Parent driver
  //
  Status = RegisterToDispatchDriver ();
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

EFI_STATUS
RegisterToDispatchDriver (
  VOID
  )
/*++

Routine Description:

  Register to dispatch driver.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Successfully init the device.
  Other        -  Error occured whening calling Dxe lib functions.

--*/
{
  UINTN                         Length;
  EFI_STATUS                    Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT   *EntryDispatchContext;
  EFI_SMM_SW_REGISTER_CONTEXT   *SwContext;
  EFI_SMM_SW_REGISTER_CONTEXT   *AcpiDisableSwContext;
  EFI_SMM_SW_REGISTER_CONTEXT   *AcpiEnableSwContext;

  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmSwDispatch2ProtocolGuid,
                  NULL,
                  (VOID **) &SwDispatch
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Length = sizeof (EFI_SMM_SW_REGISTER_CONTEXT) * 2;
  Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      Length,
                      (VOID **) &EntryDispatchContext
                      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SetMem (EntryDispatchContext, Length, 0);

  SwContext = (EFI_SMM_SW_REGISTER_CONTEXT *)EntryDispatchContext;
  AcpiDisableSwContext = SwContext++;
  AcpiEnableSwContext  = SwContext++;

  //
  // Register the enable handler
  //
  AcpiEnableSwContext->SwSmiInputValue = EFI_ACPI_ACPI_ENABLE;
  Status = SwDispatch->Register (
                        SwDispatch,
                        EnableAcpiCallback,
                        AcpiEnableSwContext,
                        &(mAcpiSmm.DisableAcpiHandle)
                        );

  //
  // Register the disable handler
  //
  AcpiDisableSwContext->SwSmiInputValue = EFI_ACPI_ACPI_DISABLE;
  Status = SwDispatch->Register (
                        SwDispatch,
                        DisableAcpiCallback,
                        AcpiDisableSwContext,
                        &(mAcpiSmm.EnableAcpiHandle)
                        );

  return Status;
}

EFI_STATUS
DisableAcpiCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  )
/*++

Routine Description:
  SMI handler to disable ACPI mode

  Dispatched on reads from APM port with value 0xA1

  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then disabled.
   Clear all ACPI event status and disable all ACPI events
   Disable PM sources except power button
   Clear status bits
   Disable GPE0 sources
   Clear status bits
   Disable GPE1 sources
   Clear status bits
   Disable SCI

Arguments:
  DispatchHandle  - EFI Handle
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

Returns:
  Nothing

--*/
{
  EFI_STATUS  Status;
  UINT16      Pm1Cnt;

  Status = GetAllQncPmBase (gSmst);
  ASSERT_EFI_ERROR (Status);
  Pm1Cnt = IoRead16 (mAcpiSmm.QncPmBase + R_QNC_PM1BLK_PM1C);

  //
  // Disable SCI
  //
  Pm1Cnt &= ~B_QNC_PM1BLK_PM1C_SCIEN;

  IoWrite16 (mAcpiSmm.QncPmBase + R_QNC_PM1BLK_PM1C, Pm1Cnt);

  return EFI_SUCCESS;
}

EFI_STATUS
EnableAcpiCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  )
/*++

Routine Description:
  SMI handler to enable ACPI mode

  Dispatched on reads from APM port with value 0xA0

  Disables the SW SMI Timer.
  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then enabled.

   Disable SW SMI Timer

   Clear all ACPI event status and disable all ACPI events
   Disable PM sources except power button
   Clear status bits

   Disable GPE0 sources
   Clear status bits

   Disable GPE1 sources
   Clear status bits

   Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)

   Enable SCI

Arguments:
  DispatchHandle  - EFI Handle
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

Returns:
  Nothing

--*/
{
  EFI_STATUS  Status;
  UINT32      SmiEn;
  UINT16      Pm1Cnt;
  UINT8       Data8;

  Status  = GetAllQncPmBase (gSmst);
  ASSERT_EFI_ERROR (Status);

  SmiEn = IoRead32 (mAcpiSmm.QncGpe0Base + R_QNC_GPE0BLK_SMIE);

  //
  // Disable SW SMI Timer
  //
  SmiEn &= ~(B_QNC_GPE0BLK_SMIE_SWT);
  IoWrite32 (mAcpiSmm.QncGpe0Base + R_QNC_GPE0BLK_SMIE, SmiEn);

  //
  // Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  //
  Data8 = RTC_ADDRESS_REGISTER_D;
  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, Data8);
  Data8 = 0x0;
  IoWrite8 (R_IOPORT_CMOS_STANDARD_DATA, Data8);

  //
  // Enable SCI
  //
  Pm1Cnt = IoRead16 (mAcpiSmm.QncPmBase + R_QNC_PM1BLK_PM1C);
  Pm1Cnt |= B_QNC_PM1BLK_PM1C_SCIEN;
  IoWrite16 (mAcpiSmm.QncPmBase + R_QNC_PM1BLK_PM1C, Pm1Cnt);

  //
  // Do platform specific stuff for ACPI enable SMI
  //


  return EFI_SUCCESS;
}

EFI_STATUS
GetAllQncPmBase (
  IN EFI_SMM_SYSTEM_TABLE2       *Smst
  )
/*++

Routine Description:

  Get QNC chipset LPC Power Management I/O Base at runtime.

Arguments:

  Smst  -  The standard SMM system table.

Returns:

  EFI_SUCCESS  -  Successfully init the device.
  Other        -  Error occured whening calling Dxe lib functions.

--*/
{
  mAcpiSmm.QncPmBase    = PciRead16 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_QNC, PCI_DEVICE_NUMBER_QNC_LPC, 0, R_QNC_LPC_PM1BLK)) & B_QNC_LPC_PM1BLK_MASK;
  mAcpiSmm.QncGpe0Base  = PciRead16 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_QNC, PCI_DEVICE_NUMBER_QNC_LPC, 0, R_QNC_LPC_GPE0BLK)) & B_QNC_LPC_GPE0BLK_MASK;

  //
  // Quark does not support Changing Primary SoC IOBARs from what was
  // setup in SEC/PEI UEFI stages.
  //
  ASSERT (mAcpiSmm.QncPmBase == (UINT32) PcdGet16 (PcdPm1blkIoBaseAddress));
  ASSERT (mAcpiSmm.QncGpe0Base == (UINT32) PcdGet16 (PcdGpe0blkIoBaseAddress));
  return EFI_SUCCESS;
}

