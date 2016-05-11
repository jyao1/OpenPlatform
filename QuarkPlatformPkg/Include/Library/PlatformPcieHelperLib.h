/** @file
PlatformPcieHelperLib function prototype definitions.

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __PLATFORM_PCIE_HELPER_LIB_H__
#define __PLATFORM_PCIE_HELPER_LIB_H__

#include "Platform.h"

//
// Function prototypes for routines exported by this library.
//

/**
  Platform assert PCI express PERST# signal.

  @param   PciExpPerstResumeWellGpio     PCIEXP_PERST_RESUMEWELL_GPIO number.

**/
VOID
EFIAPI
PlatformPERSTAssert (
  IN CONST UINT32              PciExpPerstResumeWellGpio
  );

/**
  Platform de assert PCI express PERST# signal.

  @param   PciExpPerstResumeWellGpio     PCIEXP_PERST_RESUMEWELL_GPIO number.

**/
VOID
EFIAPI
PlatformPERSTDeAssert (
  IN CONST UINT32              PciExpPerstResumeWellGpio
  );

/** Early initialisation of the PCIe controller.

  @retval   EFI_SUCCESS               Operation success.

**/
EFI_STATUS
EFIAPI
PlatformPciExpressEarlyInit (
  VOID
  );

#endif // #ifndef __PLATFORM_PCIE_HELPER_LIB_H__
