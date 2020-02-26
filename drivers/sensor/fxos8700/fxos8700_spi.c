/*
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
 * Copyright (c) 2018 Phytec Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "fxos8700.h"
#include <logging/log.h>
#include <string.h>

#ifdef DT_NXP_FXOS8700_BUS_SPI

#define FXOS8700_SPI_WRITE (1 << 7)

LOG_MODULE_DECLARE(FXOS8700, CONFIG_SENSOR_LOG_LEVEL);

static struct spi_config fxos8700_spi_conf = {
	.frequency = DT_INST_0_NXP_FXOS8700_SPI_MAX_FREQUENCY,
	.operation = (SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8) |
		      SPI_LINES_SINGLE),
	.slave = DT_INST_0_NXP_FXOS8700_BASE_ADDRESS,
	.cs = NULL,
};

int fxos8700_read(struct device *bus, u8_t reg, u8_t *data, u16_t len)
{
	struct spi_config *spi_cfg = &fxos8700_spi_conf;
	u8_t buffer_tx[2] = { reg & ~FXOS8700_SPI_WRITE, (reg & 0x80) >> 7 };
	const struct spi_buf tx_buf = {
		.buf = buffer_tx,
		.len = 2,
	};
	const struct spi_buf_set tx = { .buffers = &tx_buf, .count = 1 };
	const struct spi_buf rx_buf[2] = { {
						   .buf = NULL,
						   .len = 2,
					   },
					   {
						   .buf = data,
						   .len = len,
					   } };
	const struct spi_buf_set rx = { .buffers = rx_buf, .count = 2 };

	if (spi_transceive(bus, spi_cfg, &tx, &rx)) {
		return -EIO;
	}

	return 0;
}

int fxos8700_write(struct device *bus, u8_t reg, u8_t *data, u16_t len)
{
	struct spi_config *spi_cfg = &fxos8700_spi_conf;
	u8_t buffer_tx[2] = { reg | FXOS8700_SPI_WRITE, (reg & 0x80) >> 7 };
	const struct spi_buf tx_buf[2] = { {
						   .buf = buffer_tx,
						   .len = 2,
					   },
					   {
						   .buf = data,
						   .len = len,
					   } };
	const struct spi_buf_set tx = { .buffers = tx_buf, .count = 2 };

	if (spi_write(bus, spi_cfg, &tx)) {
		return -EIO;
	}

	return 0;
}

#if defined(DT_INST_0_NXP_FXOS8700_CS_GPIOS_CONTROLLER)
int fxos8700_spi_cs_gpios_init(struct fxos8700_data *data)
{
	/* handle SPI CS thru GPIO if it is the case */
	data->cs_ctrl.gpio_dev =
		device_get_binding(DT_INST_0_NXP_FXOS8700_CS_GPIOS_CONTROLLER);
	if (!data->cs_ctrl.gpio_dev) {
		LOG_ERR("Unable to get GPIO SPI CS device");
		return -ENODEV;
	}

	data->cs_ctrl.gpio_pin = DT_INST_0_NXP_FXOS8700_CS_GPIOS_PIN;
	data->cs_ctrl.delay = 0U;

	fxos8700_spi_conf.cs = &data->cs_ctrl;

	LOG_DBG("SPI GPIO CS configured on %s:%u",
		DT_INST_0_NXP_FXOS8700_CS_GPIOS_CONTROLLER,
		DT_INST_0_NXP_FXOS8700_CS_GPIOS_PIN);

	//TODO use ifdef and add to KConfig
//	struct device *vdd = device_get_binding("GPIO_0");
//	if (!vdd) {
//		LOG_ERR("Could not find vdd GPIO device");
//		return -EINVAL;
//	}

//	gpio_pin_configure(vdd, 25, GPIO_DIR_OUT);
//	gpio_pin_write(vdd, 25, 0);
//	k_busy_wait(USEC_PER_MSEC);

	return 0;
}
#endif /* DT_INST_0_NXP_FXOS8700_CS_GPIOS_CONTROLLER */

#endif /* DT_NXP_FXOS8700_BUS_SPI */