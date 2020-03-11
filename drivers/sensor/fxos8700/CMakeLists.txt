# Makefile - AUGU_FXOS8700 6-axis accelerometer/magnetometer
#
# Copyright (c) 2016, Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier: Apache-2.0
#

zephyr_library_named("augufxos8700")

message(STATUS "before DTS_ROOT: " ${DTS_ROOT})

list(APPEND
  DTS_ROOT
  ${CMAKE_CURRENT_LIST_DIR}
  )

message(STATUS "after DTS_ROOT: " ${DTS_ROOT})

zephyr_library_sources_ifdef(CONFIG_AUGU_FXOS8700 augufxos8700.c)
zephyr_library_sources_ifdef(CONFIG_AUGU_FXOS8700 augufxos8700_spi.c)
zephyr_library_sources_ifdef(CONFIG_AUGU_FXOS8700 augufxos8700_i2c.c)
zephyr_library_sources_ifdef(CONFIG_AUGU_FXOS8700_TRIGGER augufxos8700_trigger.c)
