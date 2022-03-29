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
#include "mraa/spi.h"

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
        fprintf(stderr, "Failed to initialize SPI\n");
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
        fprintf(stdout, "Failed to set SPI Device to 8Bit mode\n");
        goto err_exit;
    }

    uint8_t data[2] = {0x82, 0x00};
    uint8_t rxbuf[2] = {0x00, 0x00};
    int length = 2;

    /* Example AD7768-specific transfer */
    status = mraa_spi_transfer_buf(spi, data, rxbuf, length);

    if (status != MRAA_SUCCESS) {
        fprintf(stdout, "Failed to make first transfer\n");
        goto err_exit;
    }

    /* Example AD7768-specific transfer */
    status = mraa_spi_transfer_buf(spi, data, rxbuf, length);

    if (status != MRAA_SUCCESS) {
        fprintf(stdout, "Failed to make second transfer\n");
        goto err_exit;
    }

    printf("AD7768 response:\n");
    printf("<%08X>\n", rxbuf[1]);

    /* stop spi */
    mraa_spi_stop(spi);

    printf("Success!\n");
    exit(EXIT_SUCCESS);

err_exit:
    printf("Fail!\n");
    exit(EXIT_FAILURE);
}


