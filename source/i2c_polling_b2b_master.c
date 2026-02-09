/*
 * Clean TCS34725 I2C example for FRDM-RW612
 * Uses ONLY I2C_MasterTransferBlocking()
 */

#include <stdio.h>
#include <string.h>

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Configuration
 ******************************************************************************/
#define I2C_FLEXCOMM_ID     0
#define I2C_BASE            I2C0
#define I2C_BAUDRATE        100000U

/* TCS34725 */
#define TCS34725_ADDR       0x29
#define CMD_BIT             0x80

#define REG_ENABLE          0x00
#define REG_ATIME           0x01
#define REG_ID              0x12
#define REG_CDATAL          0x14

#define ENABLE_PON          0x01
#define ENABLE_AEN          0x02

/*******************************************************************************
 * I2C helpers (TRANSFER-BASED, SAFE)
 ******************************************************************************/
static status_t i2c_write(uint8_t reg, const uint8_t *data, size_t len)
{
    uint8_t buf[1 + 8]; /* enough for this sensor */
    i2c_master_transfer_t xfer = {0};

    buf[0] = CMD_BIT | reg;
    memcpy(&buf[1], data, len);

    xfer.slaveAddress = TCS34725_ADDR;
    xfer.direction    = kI2C_Write;
    xfer.data         = buf;
    xfer.dataSize     = len + 1;
    xfer.flags        = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(I2C_BASE, &xfer);
}

static status_t i2c_read(uint8_t reg, uint8_t *data, size_t len)
{
    uint8_t cmd = CMD_BIT | reg;
    i2c_master_transfer_t xfer = {0};
    status_t s;

    /* Write register pointer */
    xfer.slaveAddress = TCS34725_ADDR;
    xfer.direction    = kI2C_Write;
    xfer.data         = &cmd;
    xfer.dataSize     = 1;
    xfer.flags        = kI2C_TransferNoStopFlag;

    s = I2C_MasterTransferBlocking(I2C_BASE, &xfer);
    if (s != kStatus_Success)
        return s;

    /* Repeated START + read */
    xfer.direction = kI2C_Read;
    xfer.data      = data;
    xfer.dataSize  = len;
    xfer.flags     = kI2C_TransferRepeatedStartFlag;

    return I2C_MasterTransferBlocking(I2C_BASE, &xfer);
}

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    i2c_master_config_t cfg;
    uint32_t i2c_clk;
    uint8_t id;
    uint8_t enable;
    uint8_t raw[8];

    /* ---- Safe startup ---- */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("\r\nTCS34725 I2C Example (FRDM-RW612)\r\n");

    /* ---- Flexcomm0 clocking ---- */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM0);        /* 16 MHz */
    CLOCK_EnableClock(kCLOCK_Flexcomm0);
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);

    i2c_clk = CLOCK_GetFlexCommClkFreq(I2C_FLEXCOMM_ID);
    PRINTF("Flexcomm0 clock = %u Hz\r\n", i2c_clk);

    if (i2c_clk == 0)
    {
        PRINTF("ERROR: I2C clock is 0\r\n");
        while (1);
    }

    /* ---- I2C init ---- */
    I2C_MasterGetDefaultConfig(&cfg);
    cfg.baudRate_Bps = I2C_BAUDRATE;
    I2C_MasterInit(I2C_BASE, &cfg, i2c_clk);

    /* ---- Probe sensor ID ---- */
    if (i2c_read(REG_ID, &id, 1) == kStatus_Success)
    {
        PRINTF("TCS34725 ID = 0x%02X\r\n", id);
    }
    else
    {
        PRINTF("ERROR: Failed to read ID\r\n");
        while (1);
    }

    /* ---- Power ON ---- */
    enable = ENABLE_PON;
    i2c_write(REG_ENABLE, &enable, 1);
    SDK_DelayAtLeastUs(3000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    enable = ENABLE_PON | ENABLE_AEN;
    i2c_write(REG_ENABLE, &enable, 1);

    PRINTF("TCS34725 enabled\r\n");

    /* ---- Main loop ---- */
    while (1)
    {
        if (i2c_read(REG_CDATAL, raw, 8) == kStatus_Success)
        {
            uint16_t c = (raw[1] << 8) | raw[0];
            uint16_t r = (raw[3] << 8) | raw[2];
            uint16_t g = (raw[5] << 8) | raw[4];
            uint16_t b = (raw[7] << 8) | raw[6];

            PRINTF("C:%5u  R:%5u  G:%5u  B:%5u\r\n", c, r, g, b);
        }
        else
        {
            PRINTF("I2C read error\r\n");
        }

        SDK_DelayAtLeastUs(500000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}
