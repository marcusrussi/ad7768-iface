/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mraa/spi.h>
#include "log.h"

#define SPI_BUS 0
#define SPI_CS 0

#define SPI_FREQ 500000

int main(int argc, char *argv[])
{
    mraa_spi_context spi;
    mraa_result_t status;

    /* initialize SPI bus */
    spi = mraa_spi_init_raw(SPI_BUS, SPI_CS);
    if (spi == NULL) {
	log_error("Failed to initialize SPI");
        mraa_deinit();
        return EXIT_FAILURE;
    }
    /* set SPI frequency */
    status = mraa_spi_frequency(spi, SPI_FREQ);
    if (status != MRAA_SUCCESS)
        goto err_exit;
    /* set big endian mode */
    status = mraa_spi_lsbmode(spi, 0);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }
    status = mraa_spi_bit_per_word(spi, 8);
    if (status != MRAA_SUCCESS) {
        log_error("Failed to set SPI Device to 8Bit mode");
        goto err_exit;
    }

    // Register query
    uint8_t data_read9[4] = {0x89, 0x00, 0x89, 0x00};
    uint8_t data_reset[4] = {0x06, 0x03, 0x06, 0x02};
    uint8_t rxbuf[4] = {0x00, 0x00, 0x00, 0x00};
    int length = 2;

    log_info("Reading status register @ 0x09...0x04 means no clock detected");
    status = mraa_spi_transfer_buf(spi, data_read9, rxbuf, length);
    status = mraa_spi_transfer_buf(spi, data_read9 + 2, rxbuf + 2, length);

    if (status != MRAA_SUCCESS) {
        log_error("Failed to make first transfer");
        goto err_exit;
    }

    log_debug("AD7768 response:");
    log_debug("<%02X>, <%02X>, <%02X>, <%02X>", rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3]);

    // Soft reset
    /* Perform the soft reset */
    log_info("Resetting AD7768");
    status = mraa_spi_transfer_buf(spi, data_reset, rxbuf, length);
    status = mraa_spi_transfer_buf(spi, data_reset + 2, rxbuf + 2, length);

    if (status != MRAA_SUCCESS) {
        log_error("Failed to make first transfer");
        goto err_exit;
    }

    usleep(10000);

    log_debug("AD7768 reset response:");
    log_debug("<%02X>, <%02X>, <%02X>, <%02X>", rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3]);

    /* Read the external clock check register */
    log_info("Reading status register @ 0x09...0x04 means no clock detected");
    status = mraa_spi_transfer_buf(spi, data_read9, rxbuf, length);
    status = mraa_spi_transfer_buf(spi, data_read9 + 2, rxbuf + 2, length);
    
    if (status != MRAA_SUCCESS) {
        log_error("Failed to make second transfer");
        goto err_exit;
    }

    log_debug("AD7768 response:");
    log_debug("<%02X>, <%02X>, <%02X>, <%02X>", rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3]);

    /* stop spi */
    mraa_spi_stop(spi);

    log_info("Success!");
    exit(EXIT_SUCCESS);

err_exit:
    log_fatal("Fail!");
    exit(EXIT_FAILURE);
}


