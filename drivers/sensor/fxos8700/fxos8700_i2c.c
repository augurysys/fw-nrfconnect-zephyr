/*
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
 * Copyright (c) 2018 Phytec Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <drivers/i2c.h>
#include <logging/log.h>

#include "fxos8700.h"

#ifdef DT_NXP_FXOS8700_BUS_I2C

static u16_t fxos8700_i2c_slave_addr = DT_INST_0_NXP_FXOS8700_BASE_ADDRESS;

LOG_MODULE_DECLARE(FXOS8700, CONFIG_SENSOR_LOG_LEVEL);

inline int fxos8700_read(struct device *i2c, u8_t reg_addr,
				 u8_t *value, u16_t len)
{
	return i2c_burst_read(i2c, fxos8700_i2c_slave_addr,
			      reg_addr, value, len);
}

inline int fxos8700_write(struct device *i2c, u8_t reg_addr,
				  u8_t *value, u16_t len)
{
	return i2c_burst_write(i2c, fxos8700_i2c_slave_addr,
			       reg_addr, value, len);
}
#endif /* DT_NXP_FXOS8700_BUS_I2C */
