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

#ifndef __BOARDS_GALILEO_GEN2_H__
#define __BOARDS_GALILEO_GEN2_H__

#define GALILEO_GEN2_LEGACY_GPIO_INITIALIZER            {0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x3f,0x1c,0x02,0x00,0x00,0x00,0x00,0x3f,0x00}
#define GALILEO_GEN2_GPIO_CONTROLLER_INITIALIZER        {0x05,0x05,0,0,0,0,0,0}

//
// Legacy Gpio to be used to assert / deassert PCI express PERST# signal
// on Galileo Gen 2 platform.
//
#define GALILEO_GEN2_PCIEXP_PERST_RESUMEWELL_GPIO       0

//
// Io expander slave address.
//

//
// Three IO Expmanders at fixed addresses on Galileo Gen2.
//
#define GALILEO_GEN2_IOEXP0_7BIT_SLAVE_ADDR             0x25
#define GALILEO_GEN2_IOEXP1_7BIT_SLAVE_ADDR             0x26
#define GALILEO_GEN2_IOEXP2_7BIT_SLAVE_ADDR             0x27

//
// Led GPIOs for flash update / recovery.
//
#define GALILEO_GEN2_FLASH_UPDATE_LED_RESUMEWELL_GPIO   5

#endif
