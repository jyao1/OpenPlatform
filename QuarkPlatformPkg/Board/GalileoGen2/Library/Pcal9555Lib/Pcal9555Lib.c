/** @file
Helper routines with common PEI / DXE implementation.

Copyright (c) 2013-2016 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Library/I2cLib.h>

#include <GalileoGen2/Include/Library/Pcal9555Lib.h>

//
// Routines defined in other source modules of this component.
//

//
// Routines local to this source module.
//

//
// Routines shared with other souce modules in this component.
//


BOOLEAN
Pcal9555GetPortRegBit (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum,
  IN CONST UINT8                          RegBase
  )
{
  EFI_STATUS                        Status;
  UINTN                             ReadLength;
  UINTN                             WriteLength;
  UINT8                             Data[2];
  EFI_I2C_DEVICE_ADDRESS            I2cDeviceAddr;
  EFI_I2C_ADDR_MODE                 I2cAddrMode;
  UINT8                             *RegValuePtr;
  UINT8                             GpioNumMask;
  UINT8                             SubAddr;

  I2cDeviceAddr.I2CDeviceAddress = (UINTN)Pcal9555SlaveAddr;
  I2cAddrMode = EfiI2CSevenBitAddrMode;

  if (GpioNum < 8) {
    SubAddr = RegBase;
    GpioNumMask = (UINT8)(1 << GpioNum);
  } else {
    SubAddr = RegBase + 1;
    GpioNumMask = (UINT8)(1 << (GpioNum - 8));
  }

  //
  // Output port value always at 2nd byte in Data variable.
  //
  RegValuePtr = &Data[1];

  //
  // On read entry sub address at 2nd byte, on read exit output
  // port value in 2nd byte.
  //
  Data[1] = SubAddr;
  WriteLength = 1;
  ReadLength = 1;
  Status = I2cReadMultipleByte (
    I2cDeviceAddr,
    I2cAddrMode,
    &WriteLength,
    &ReadLength,
    &Data[1]
    );
  ASSERT_EFI_ERROR (Status);

  //
  // Adjust output port bit given callers request.
  //
  return ((*RegValuePtr & GpioNumMask) != 0);
}

VOID
Pcal9555SetPortRegBit (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum,
  IN CONST UINT8                          RegBase,
  IN CONST BOOLEAN                        LogicOne
  )
{
  EFI_STATUS                        Status;
  UINTN                             ReadLength;
  UINTN                             WriteLength;
  UINT8                             Data[2];
  EFI_I2C_DEVICE_ADDRESS            I2cDeviceAddr;
  EFI_I2C_ADDR_MODE                 I2cAddrMode;
  UINT8                             *RegValuePtr;
  UINT8                             GpioNumMask;
  UINT8                             SubAddr;

  I2cDeviceAddr.I2CDeviceAddress = (UINTN)Pcal9555SlaveAddr;
  I2cAddrMode = EfiI2CSevenBitAddrMode;

  if (GpioNum < 8) {
    SubAddr = RegBase;
    GpioNumMask = (UINT8)(1 << GpioNum);
  } else {
    SubAddr = RegBase + 1;
    GpioNumMask = (UINT8)(1 << (GpioNum - 8));
  }

  //
  // Output port value always at 2nd byte in Data variable.
  //
  RegValuePtr = &Data[1];

  //
  // On read entry sub address at 2nd byte, on read exit output
  // port value in 2nd byte.
  //
  Data[1] = SubAddr;
  WriteLength = 1;
  ReadLength = 1;
  Status = I2cReadMultipleByte (
    I2cDeviceAddr,
    I2cAddrMode,
    &WriteLength,
    &ReadLength,
    &Data[1]
    );
  ASSERT_EFI_ERROR (Status);

  //
  // Adjust output port bit given callers request.
  //
  if (LogicOne) {
    *RegValuePtr = *RegValuePtr | GpioNumMask;
  } else {
    *RegValuePtr = *RegValuePtr & ~(GpioNumMask);
  }

  //
  // Update register. Sub address at 1st byte, value at 2nd byte.
  //
  WriteLength = 2;
  Data[0] = SubAddr;
  Status = I2cWriteMultipleByte (
    I2cDeviceAddr,
    I2cAddrMode,
    &WriteLength,
    Data
    );
  ASSERT_EFI_ERROR (Status);
}

/**
Set the direction of Pcal9555 IO Expander GPIO pin.

@param  Pcal9555SlaveAddr  I2c Slave address of Pcal9555 Io Expander.
@param  GpioNum            Gpio direction to configure - values 0-7 for Port0
and 8-15 for Port1.
@param  CfgAsInput         If TRUE set pin direction as input else set as output.

**/
VOID
EFIAPI
PlatformPcal9555GpioSetDir (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum,
  IN CONST BOOLEAN                        CfgAsInput
  )
{
  Pcal9555SetPortRegBit (
    Pcal9555SlaveAddr,
    GpioNum,
    PCAL9555_REG_CFG_PORT0,
    CfgAsInput
    );
}

/**
Set the level of Pcal9555 IO Expander GPIO high or low.

@param  Pcal9555SlaveAddr  I2c Slave address of Pcal9555 Io Expander.
@param  GpioNum            Gpio to change values 0-7 for Port0 and 8-15
for Port1.
@param  HighLevel          If TRUE set pin high else set pin low.

**/
VOID
EFIAPI
PlatformPcal9555GpioSetLevel (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum,
  IN CONST BOOLEAN                        HighLevel
  )
{
  Pcal9555SetPortRegBit (
    Pcal9555SlaveAddr,
    GpioNum,
    PCAL9555_REG_OUT_PORT0,
    HighLevel
    );
}

/**

Enable pull-up/pull-down resistors of Pcal9555 GPIOs.

@param  Pcal9555SlaveAddr  I2c Slave address of Pcal9555 Io Expander.
@param  GpioNum            Gpio to change values 0-7 for Port0 and 8-15
for Port1.

**/
VOID
EFIAPI
PlatformPcal9555GpioEnablePull (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum
  )
{
  Pcal9555SetPortRegBit (
    Pcal9555SlaveAddr,
    GpioNum,
    PCAL9555_REG_PULL_EN_PORT0,
    TRUE
    );
}

/**

Disable pull-up/pull-down resistors of Pcal9555 GPIOs.

@param  Pcal9555SlaveAddr  I2c Slave address of Pcal9555 Io Expander.
@param  GpioNum            Gpio to change values 0-7 for Port0 and 8-15
for Port1.

**/
VOID
EFIAPI
PlatformPcal9555GpioDisablePull (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum
  )
{
  Pcal9555SetPortRegBit (
    Pcal9555SlaveAddr,
    GpioNum,
    PCAL9555_REG_PULL_EN_PORT0,
    FALSE
    );
}

/**

Get state of Pcal9555 GPIOs.

@param  Pcal9555SlaveAddr  I2c Slave address of Pcal9555 Io Expander.
@param  GpioNum            Gpio to change values 0-7 for Port0 and 8-15
for Port1.

@retval TRUE               GPIO pin is high
@retval FALSE              GPIO pin is low
**/
BOOLEAN
EFIAPI
PlatformPcal9555GpioGetState (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum
  )
{
  return Pcal9555GetPortRegBit (Pcal9555SlaveAddr, GpioNum, PCAL9555_REG_IN_PORT0);
}


