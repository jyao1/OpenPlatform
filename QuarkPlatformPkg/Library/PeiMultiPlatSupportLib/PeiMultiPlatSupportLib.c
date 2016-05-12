/** @file

Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under
the terms and conditions of the BSD License that accompanies this distribution.
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MultiPlatSupportLib.h>
#include <Library/HobLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Guid/MultiPlatSupport.h>

extern EFI_PEI_NOTIFY_DESCRIPTOR mMemoryNotifyList;

/**

  This function finds the matched default data and create GUID hob for it. 

  @param[in] DefaultId  Specifies the type of defaults to retrieve.
  @param[in] BoardId    Specifies the platform board of defaults to retrieve.

  @retval EFI_SUCCESS           The matched default data is found.
  @retval EFI_NOT_FOUND         The matched default data is not found.
  @retval EFI_OUT_OF_RESOURCES  No enough resource to create HOB.

**/
EFI_STATUS
EFIAPI
CreateDefaultVariableHob (
  IN UINT16  DefaultId,
  IN UINT16  BoardId
  )
{
  UINTN                      FvInstance;
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
  EFI_FFS_FILE_HEADER        *FfsHeader;
  UINT32                     FileSize;
  EFI_COMMON_SECTION_HEADER  *Section;
  UINT32                     SectionLength;
  EFI_STATUS                 Status;
  BOOLEAN                    DefaultFileIsFound;
  DEFAULT_DATA               *DefaultData;
  DEFAULT_INFO               *DefaultInfo;
  VARIABLE_STORE_HEADER      *VarStoreHeader;
  VARIABLE_STORE_HEADER      *VarStoreHeaderHob;
  UINT8                      *VarHobPtr;
  UINT8                      *VarPtr;
  UINT32                     VarDataOffset;
  UINT32                     VarHobDataOffset;
  EFI_BOOT_MODE              BootMode;
  CONST EFI_PEI_SERVICES     **PeiServices;

  //
  // Get PeiService pointer
  //
  PeiServices = GetPeiServicesTablePointer ();

  //
  // Find the FFS file that stores all default data.
  //
  DefaultFileIsFound = FALSE;
  FvInstance         = 0;
  FfsHeader          = NULL;
  while (((*PeiServices)->FfsFindNextVolume (PeiServices, FvInstance, &FvHeader) == EFI_SUCCESS) &&
         (!DefaultFileIsFound)) {
    FfsHeader = NULL;
    while ((*PeiServices)->FfsFindNextFile (PeiServices, EFI_FV_FILETYPE_FREEFORM, FvHeader, &FfsHeader) == EFI_SUCCESS) {
      if (CompareGuid ((EFI_GUID *) FfsHeader, &gDefaultDataFileGuid)) {
        DefaultFileIsFound = TRUE;
        break;
      }
    }
    FvInstance ++;
  }

  //
  // FFS file is not found.
  //
  if (!DefaultFileIsFound) {
    return EFI_NOT_FOUND;
  }

  //
  // Find the matched default data for the input default ID and plat ID.
  //
  VarStoreHeader = NULL;
  Section  = (EFI_COMMON_SECTION_HEADER *)(FfsHeader + 1);
  FileSize = *(UINT32 *)(FfsHeader->Size) & 0x00FFFFFF;
  while (((UINTN) Section < (UINTN) FfsHeader + FileSize) && (VarStoreHeader == NULL)) {
    DefaultData = (DEFAULT_DATA *) (Section + 1);
    DefaultInfo = &(DefaultData->DefaultInfo[0]);
    while ((UINTN) DefaultInfo < (UINTN) DefaultData + DefaultData->HeaderSize) {
      if (DefaultInfo->DefaultId == DefaultId && DefaultInfo->BoardId == BoardId) {
        VarStoreHeader = (VARIABLE_STORE_HEADER *) ((UINT8 *) DefaultData + DefaultData->HeaderSize);
        break;
      }
      DefaultInfo ++;
    }
    //
    // Size is 24 bits wide so mask upper 8 bits. 
    // SectionLength is adjusted it is 4 byte aligned.
    // Go to the next section
    //
    SectionLength = *(UINT32 *)Section->Size & 0x00FFFFFF;
    SectionLength = (SectionLength + 3) & (~3);
    ASSERT (SectionLength != 0);
    Section = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Section + SectionLength);
  }
  //
  // Matched default data is not found.
  //
  if (VarStoreHeader == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Create HOB to store default data so that Variable driver can use it.
  // Allocate more data for header alignment.
  //
  VarStoreHeaderHob = (VARIABLE_STORE_HEADER *) BuildGuidHob (&VarStoreHeader->Signature, VarStoreHeader->Size + HEADER_ALIGNMENT - 1);
  if (VarStoreHeaderHob == NULL) {
    //
    // No enough hob resource.
    //
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Copy variable storage header.
  //
  CopyMem (VarStoreHeaderHob, VarStoreHeader, sizeof (VARIABLE_STORE_HEADER));
  //
  // Copy variable data.
  //
  VarPtr           = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeader + 1));
  VarDataOffset    = (UINT32) ((UINTN) VarPtr - (UINTN) VarStoreHeader);
  VarHobPtr        = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeaderHob + 1));
  VarHobDataOffset = (UINT32) ((UINTN) VarHobPtr - (UINTN) VarStoreHeaderHob);
  CopyMem (VarHobPtr, VarPtr, VarStoreHeader->Size - VarDataOffset);
  //
  // Update variable size.
  //
  VarStoreHeaderHob->Size = VarStoreHeader->Size - VarDataOffset + VarHobDataOffset;

  //
  // On recovery boot mode, emulation variable driver will be used.
  // But, Emulation variable only knows normal variable data format. 
  // So, if the default variable data format is authenticated, it needs to be converted to normal data.
  //
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (BootMode == BOOT_IN_RECOVERY_MODE && 
      CompareGuid (&VarStoreHeader->Signature, &gEfiAuthenticatedVariableGuid)) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mMemoryNotifyList);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
