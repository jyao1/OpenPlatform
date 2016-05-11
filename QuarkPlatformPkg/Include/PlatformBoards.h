/** @file
Board config definitions for each of the boards supported by this platform
package.

Copyright (c) 2013 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/
#include "Platform.h"

#ifndef __PLATFORM_BOARDS_H__
#define __PLATFORM_BOARDS_H__

//
// Constant definition
//

//
// Io expander slave address.
//

//
// Legacy GPIO config struct for each element in PLATFORM_LEGACY_GPIO_TABLE_DEFINITION.
//
typedef struct {
  UINT32  CoreWellEnable;               ///< Value for QNC NC Reg R_QNC_GPIO_CGEN_CORE_WELL.
  UINT32  CoreWellIoSelect;             ///< Value for QNC NC Reg R_QNC_GPIO_CGIO_CORE_WELL.
  UINT32  CoreWellLvlForInputOrOutput;  ///< Value for QNC NC Reg R_QNC_GPIO_CGLVL_CORE_WELL.
  UINT32  CoreWellTriggerPositiveEdge;  ///< Value for QNC NC Reg R_QNC_GPIO_CGTPE_CORE_WELL.
  UINT32  CoreWellTriggerNegativeEdge;  ///< Value for QNC NC Reg R_QNC_GPIO_CGTNE_CORE_WELL.
  UINT32  CoreWellGPEEnable;            ///< Value for QNC NC Reg R_QNC_GPIO_CGGPE_CORE_WELL.
  UINT32  CoreWellSMIEnable;            ///< Value for QNC NC Reg R_QNC_GPIO_CGSMI_CORE_WELL.
  UINT32  CoreWellTriggerStatus;        ///< Value for QNC NC Reg R_QNC_GPIO_CGTS_CORE_WELL.
  UINT32  CoreWellNMIEnable;            ///< Value for QNC NC Reg R_QNC_GPIO_CGNMIEN_CORE_WELL.
  UINT32  ResumeWellEnable;             ///< Value for QNC NC Reg R_QNC_GPIO_RGEN_RESUME_WELL.
  UINT32  ResumeWellIoSelect;           ///< Value for QNC NC Reg R_QNC_GPIO_RGIO_RESUME_WELL.
  UINT32  ResumeWellLvlForInputOrOutput;///< Value for QNC NC Reg R_QNC_GPIO_RGLVL_RESUME_WELL.
  UINT32  ResumeWellTriggerPositiveEdge;///< Value for QNC NC Reg R_QNC_GPIO_RGTPE_RESUME_WELL.
  UINT32  ResumeWellTriggerNegativeEdge;///< Value for QNC NC Reg R_QNC_GPIO_RGTNE_RESUME_WELL.
  UINT32  ResumeWellGPEEnable;          ///< Value for QNC NC Reg R_QNC_GPIO_RGGPE_RESUME_WELL.
  UINT32  ResumeWellSMIEnable;          ///< Value for QNC NC Reg R_QNC_GPIO_RGSMI_RESUME_WELL.
  UINT32  ResumeWellTriggerStatus;      ///< Value for QNC NC Reg R_QNC_GPIO_RGTS_RESUME_WELL.
  UINT32  ResumeWellNMIEnable;          ///< Value for QNC NC Reg R_QNC_GPIO_RGNMIEN_RESUME_WELL.
} BOARD_LEGACY_GPIO_CONFIG;

//
// GPIO controller config struct for each element in PLATFORM_GPIO_CONTROLLER_CONFIG_DEFINITION.
//
typedef struct {
  UINT32  PortADR;                      ///< Value for IOH REG GPIO_SWPORTA_DR.
  UINT32  PortADir;                     ///< Value for IOH REG GPIO_SWPORTA_DDR.
  UINT32  IntEn;                        ///< Value for IOH REG GPIO_INTEN.
  UINT32  IntMask;                      ///< Value for IOH REG GPIO_INTMASK.
  UINT32  IntType;                      ///< Value for IOH REG GPIO_INTTYPE_LEVEL.
  UINT32  IntPolarity;                  ///< Value for IOH REG GPIO_INT_POLARITY.
  UINT32  Debounce;                     ///< Value for IOH REG GPIO_DEBOUNCE.
  UINT32  LsSync;                       ///< Value for IOH REG GPIO_LS_SYNC.
} BOARD_GPIO_CONTROLLER_CONFIG;

typedef
VOID
(EFIAPI *BOARD_INIT) (
  VOID
  );


#endif
