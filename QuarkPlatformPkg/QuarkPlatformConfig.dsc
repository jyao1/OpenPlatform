## @file
# Quark Platform Configuration File.
#
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

#
# The basic configuration
#
DEFINE BOOT_SHELL_ONLY   = FALSE


  #
  # Platform On/Off features are defined here
  #
  DEFINE SECURE_BOOT_ENABLE   = FALSE
  DEFINE MEASURED_BOOT_ENABLE = FALSE
  DEFINE SOURCE_DEBUG_ENABLE  = FALSE
  DEFINE PERFORMANCE_ENABLE   = FALSE
  DEFINE LOGGING              = FALSE
  #
  # Galileo board.  Options are [GEN1, GEN2]
  #
  DEFINE GALILEO              = GEN2

  #
  # TPM 1.2 Hardware.  Options are [NONE, LPC, ATMEL_I2C, INFINEON_I2C]
  #
  DEFINE TPM_12_HARDWARE      = NONE

  !if $(TARGET) == "DEBUG"
    DEFINE LOGGING = TRUE
  !endif

  !if $(PERFORMANCE_ENABLE)
    DEFINE SOURCE_DEBUG_ENABLE = FALSE
    DEFINE LOGGING             = FALSE
  !endif

