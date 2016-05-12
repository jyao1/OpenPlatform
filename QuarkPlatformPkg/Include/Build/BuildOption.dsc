## @file
# Clanton Peak CRB platform with 32-bit DXE for 4MB/8MB flash devices.
#
# This package provides Clanton Peak CRB platform specific modules.
# Copyright (c) 2013 - 2016 Intel Corporation.
#
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
##

[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
!if $(BOOT_SHELL_ONLY) == FALSE
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
!endif