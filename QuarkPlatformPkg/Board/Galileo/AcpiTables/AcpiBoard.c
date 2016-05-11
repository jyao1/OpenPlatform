/** @file
ACPI Platform Driver

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <QuarkNcSocId.h>

#include <Platform.h>
#include <PlatformBoards.h>
#include <Galileo/Include/BoardGalileo.h>
#include <Library/PlatformHelperLib.h>

#pragma pack(1)
typedef struct {
  UINT8                  NameOp;
  UINT32                 NameString;
  UINT8                  DWordPrefix;
  UINT32                 Value;
} AML_NAME_DWORD_OBJ;
#pragma pack()

EFI_GLOBAL_NVS_AREA_PROTOCOL  *mGlobalNvsArea;
UINT32                        mAlternateSla;

EFI_STATUS
LocateSupportProtocol (
  IN  EFI_GUID                       *Protocol,
  OUT VOID                           **Instance,
  IN  UINT32                         Type
  )
/*++

Routine Description:

  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

Arguments:

  Protocol      The protocol to find.
  Instance      Return pointer to the first instance of the protocol

Returns:

  EFI_SUCCESS           The function completed successfully.
  EFI_NOT_FOUND         The protocol could not be located.
  EFI_OUT_OF_RESOURCES  There are not enough resources to find the protocol.

--*/
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   i;

  FvStatus = 0;

  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                   ByProtocol,
                   Protocol,
                   NULL,
                   &NumberOfHandles,
                   &HandleBuffer
                   );
  if (EFI_ERROR (Status)) {

    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }



  //
  // Looking for FV with ACPI storage file
  //

  for (i = 0; i < NumberOfHandles; i++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                     HandleBuffer[i],
                     Protocol,
                     Instance
                     );
    ASSERT_EFI_ERROR (Status);

    if (!Type) {
      //
      // Not looking for the FV protocol, so find the first instance of the
      // protocol.  There should not be any errors because our handle buffer
      // should always contain at least one or LocateHandleBuffer would have
      // returned not found.
      //
      break;
    }

    //
    // See if it has the ACPI storage file
    //

    Status = ((EFI_FIRMWARE_VOLUME2_PROTOCOL*) (*Instance))->ReadFile (*Instance,
                                                              &gEfiCallerIdGuid,
                                                              NULL,
                                                              &Size,
                                                              &FileType,
                                                              &Attributes,
                                                              &FvStatus
                                                              );

    //
    // If we found it, then we are done
    //
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //

  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  return Status;
}

VOID
AcpiUpdateTable (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER  *TableHeader,
  IN OUT   EFI_ACPI_TABLE_VERSION       *Version
  )
/*++

  Routine Description:

    Set the correct table revision upon the setup value

  Arguments:

    Table   - The table to be set
    Version - Version to publish

  Returns:

    None

--*/

{
  //
  // By default, a table belongs in all ACPI table versions published.
  // Some tables will override this because they have different versions of the table.
  //
  *Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;

  //
  // Update the various table types with the necessary updates
  //
  switch (TableHeader->Signature) {

  case EFI_ACPI_1_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
    if (TableHeader->OemTableId == SIGNATURE_64('G', 'a', 'l', 'i', 'l', 'e', 'o', '1')) {
      AML_NAME_DWORD_OBJ             *NameObj;
      //
      // Patch some pointers for the ASL code before loading the SSDT.
      //
      for (NameObj = (AML_NAME_DWORD_OBJ *)(TableHeader + 1);
           NameObj < (AML_NAME_DWORD_OBJ *)((UINT8 *)TableHeader + TableHeader->Length);
           NameObj = (AML_NAME_DWORD_OBJ *)((UINT8 *)NameObj + 1)) {
        if ((NameObj->NameOp == AML_NAME_OP) &&
            (NameObj->NameString == SIGNATURE_32('A', 'L', 'T', 'S')) &&
            (NameObj->DWordPrefix == AML_DWORD_PREFIX) ) {
          DEBUG((EFI_D_INFO, "Patch ALTS to 0x%x\n", mAlternateSla));
          NameObj->Value = mAlternateSla;
          break;
        }
      }
    }

    break;
    
  default:
    break;
  }
  return ;
}

EFI_STATUS
PublishAcpiTablesFromFv (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINT32                        FvStatus;
  UINTN                         Size;
  EFI_ACPI_TABLE_VERSION        Version;
  UINTN                         TableHandle;
  INTN                          Instance;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;

  Instance      = 0;
  TableHandle   = 0;
  CurrentTable  = NULL;
  FwVol         = NULL;

  //
  // Find the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID**)&AcpiTable);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (&gEfiFirmwareVolume2ProtocolGuid, (VOID**)&FwVol, 1);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }
  //
  // Read tables from the storage file.
  //

  while (Status == EFI_SUCCESS) {

    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiCallerIdGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID**)&CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR(Status)) {
      //
      // Perform any table specific updates.
      //
      AcpiUpdateTable ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable, &Version);

      //
      // Add the table
      //
      TableHandle = 0;
      Status = AcpiTable->InstallAcpiTable (
                            AcpiTable,
                            CurrentTable,
                            ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->Length,
                            &TableHandle
                            );
      if (EFI_ERROR(Status)) {
        return EFI_ABORTED;
      }

      //
      // Increment the instance
      //
      Instance++;
      CurrentTable = NULL;
    }
  }

  //
  // Finished
  //
  return EFI_SUCCESS;
}

/**
  ACPI Platform driver installation function.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval EFI_ABORTED        The driver encountered an error and could not complete installation of
                             the ACPI tables.

**/
EFI_STATUS
EFIAPI
AcpiBoardEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;

  if (PcdGet16 (PcdPlatformType) != Galileo) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **)&mGlobalNvsArea
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Configure platform IO expander I2C Slave Address.
  //
  if (PlatformLegacyGpioGetLevel (R_QNC_GPIO_RGLVL_RESUME_WELL, GALILEO_DETERMINE_IOEXP_SLA_RESUMEWELL_GPIO)) {
    mAlternateSla = FALSE;
  } else {
    mAlternateSla = TRUE;
  }

  PublishAcpiTablesFromFv ();

  return EFI_SUCCESS;
}

