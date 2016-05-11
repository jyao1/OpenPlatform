/** @file
Contains root level name space objects for the board

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <QuarkAcpiGpioMapping.h>

DefinitionBlock (
  "Platform.aml",
  "SSDT",
  1,
  "INTEL ",
  "Galileo2",
  3)
{
    Scope(\_SB)
    {
        //
        // Include asi files for I2C and SPI onboard devices.
        // Devices placed here instead of below relevant controllers.
        // Hardware topology information is maintained by the
        // ResourceSource arg to the I2CSerialBus/SPISerialBus macros
        // within the device asi files.
        //
        Include ("PCAL9555A.asi")    // NXP PCAL9555A I/O expander.
        Include ("PCA9685.asi")      // NXP PCA9685 PWM/LED controller.
        Include ("CAT24C08.asi")     // ONSEMI CAT24C08 I2C 8KB EEPROM.
        Include ("ADC108S102.asi")   // TI ADC108S102 ADC.
    }
}
