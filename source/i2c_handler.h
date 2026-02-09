/*
 * i2c_handler.h
 *
 *  Created on: 12 Feb 2026
 *      Author: liren
 */

#ifndef I2C_HANDLER_H_
#define I2C_HANDLER_H_


#include "fsl_i2c.h"
#include <stdint.h>
#include <stddef.h>

/* ================= CONFIG ================= */
#define I2C_HANDLER_BASE        I2C0
#define I2C_HANDLER_FLEXCOMM    0
#define I2C_HANDLER_BAUDRATE    100000U

/* ============== API ======================= */

/* Init I2C peripheral */
void I2C_Handler_Init(void);

/* Write single register */
status_t I2C_Handler_WriteReg(uint8_t devAddr,
                              uint8_t reg,
                              uint8_t value);

/* Write multiple bytes */
status_t I2C_Handler_Write(uint8_t devAddr,
                           uint8_t reg,
                           const uint8_t *data,
                           size_t len);

/* Read multiple bytes */
status_t I2C_Handler_Read(uint8_t devAddr,
                          uint8_t reg,
                          uint8_t *data,
                          size_t len);


#endif /* I2C_HANDLER_H_ */
