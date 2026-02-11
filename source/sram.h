/*
 * sram.h
 *
 *  Created on: 10 Feb 2026
 *      Author: liren
 */

#ifndef SRAM_H_
#define SRAM_H_

#include <stdint.h>
#include <stddef.h>
#include "fsl_common.h"

#define SRAM_23LC1024_SIZE_BYTES   (128 * 1024)

status_t sram23_init(void);
status_t sram23_write(uint32_t addr, const uint8_t *buf, size_t len);
status_t sram23_read(uint32_t addr, uint8_t *buf, size_t len);

#endif /* SRAM_H_ */
