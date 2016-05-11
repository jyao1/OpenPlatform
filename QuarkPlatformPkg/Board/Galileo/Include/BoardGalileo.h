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

#ifndef __BOARDS_GALILEO_H__
#define __BOARDS_GALILEO_H__

//
// Io expander slave address.
//

//
// On Galileo value of Jumper J2 determines slave address of io expander.
//
#define GALILEO_DETERMINE_IOEXP_SLA_RESUMEWELL_GPIO     5
#define GALILEO_IOEXP_J2HI_7BIT_SLAVE_ADDR              0x20
#define GALILEO_IOEXP_J2LO_7BIT_SLAVE_ADDR              0x21

#endif
