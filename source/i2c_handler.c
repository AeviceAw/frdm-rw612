/*
 * i2c_handler.c
 *
 *  Created on: 12 Feb 2026
 *      Author: liren
 */

#include "i2c_handler.h"
#include "fsl_clock.h"
#include "fsl_reset.h"

/* ================= INIT =================== */
void I2C_Handler_Init(void)
{
    i2c_master_config_t cfg;
    uint32_t clk;

    /* Attach clock to FLEXCOMM */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_Flexcomm0);
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);

    clk = CLOCK_GetFlexCommClkFreq(I2C_HANDLER_FLEXCOMM);

    I2C_MasterGetDefaultConfig(&cfg);
    cfg.baudRate_Bps = I2C_HANDLER_BAUDRATE;

    I2C_MasterInit(I2C_HANDLER_BASE, &cfg, clk);
}

/* ============== WRITE REG ================= */
status_t I2C_Handler_WriteReg(uint8_t devAddr,
                              uint8_t reg,
                              uint8_t value)
{
    uint8_t tx[2] = {reg, value};
    i2c_master_transfer_t xfer = {0};

    xfer.slaveAddress = devAddr;
    xfer.direction    = kI2C_Write;
    xfer.data         = tx;
    xfer.dataSize     = 2;
    xfer.flags        = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(I2C_HANDLER_BASE, &xfer);
}

/* ============== WRITE MULTI =============== */
status_t I2C_Handler_Write(uint8_t devAddr,
                           uint8_t reg,
                           const uint8_t *data,
                           size_t len)
{
    uint8_t buf[32];   /* adjust if needed */
    i2c_master_transfer_t xfer = {0};

    if (len + 1 > sizeof(buf))
        return kStatus_Fail;

    buf[0] = reg;
    for (size_t i = 0; i < len; i++)
        buf[i + 1] = data[i];

    xfer.slaveAddress = devAddr;
    xfer.direction    = kI2C_Write;
    xfer.data         = buf;
    xfer.dataSize     = len + 1;
    xfer.flags        = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(I2C_HANDLER_BASE, &xfer);
}

/* ============== READ ====================== */
status_t I2C_Handler_Read(uint8_t devAddr,
                          uint8_t reg,
                          uint8_t *data,
                          size_t len)
{
    i2c_master_transfer_t xfer = {0};

    xfer.slaveAddress   = devAddr;
    xfer.direction      = kI2C_Read;
    xfer.subaddress     = reg;
    xfer.subaddressSize = 1;
    xfer.data           = data;
    xfer.dataSize       = len;
    xfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(I2C_HANDLER_BASE, &xfer);
}
