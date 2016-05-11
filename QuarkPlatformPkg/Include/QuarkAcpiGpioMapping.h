/** @file
Quark Acpi Gpio Mapping information.

Copyright (c) 2013 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/

#ifndef __QUARK_ACPI_GPIO_MAPPING_H__
#define __QUARK_ACPI_GPIO_MAPPING_H__

//
// GPIO Interrupt Connection Resource Descriptor (GpioInt) usage.
// GpioInt() descriptors maybe used in this file and included .asi files.
//
// The mapping below was provided by the first OS user that requested
// GpioInt() support.
// Other OS users that need GpioInt() support must use the following mapping.
//
#define QUARK_GPIO8_MAPPING         0x00
#define QUARK_GPIO9_MAPPING         0x01
#define QUARK_GPIO_SUS0_MAPPING     0x02
#define QUARK_GPIO_SUS1_MAPPING     0x03
#define QUARK_GPIO_SUS2_MAPPING     0x04
#define QUARK_GPIO_SUS3_MAPPING     0x05
#define QUARK_GPIO_SUS4_MAPPING     0x06
#define QUARK_GPIO_SUS5_MAPPING     0x07
#define QUARK_GPIO0_MAPPING         0x08
#define QUARK_GPIO1_MAPPING         0x09
#define QUARK_GPIO2_MAPPING         0x0A
#define QUARK_GPIO3_MAPPING         0x0B
#define QUARK_GPIO4_MAPPING         0x0C
#define QUARK_GPIO5_MAPPING         0x0D
#define QUARK_GPIO6_MAPPING         0x0E
#define QUARK_GPIO7_MAPPING         0x0F

#endif
