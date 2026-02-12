/*
 * sram.h
 *
 *  Created on: 10 Feb 2026
 *      Author: liren
 */

#ifndef SPI_SRAM_H_
#define SPI_SRAM_H_

#include <stdint.h>
#include <stddef.h>
#include "fsl_common.h"

#define SRAM_23LC1024_SIZE_BYTES   (128 * 1024)

/* Initialize SPI peripheral and CS GPIO */
void SPI_SRAM_Init(void);

/* Write bytes to SRAM */
bool SPI_SRAM_Write(uint32_t address, const uint8_t *data, uint32_t length);

/* Read bytes from SRAM */
bool SPI_SRAM_Read(uint32_t address, uint8_t *data, uint32_t length);

#endif /* SPI_SRAM_H_ */
