#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"

/* ===================== I2C DEFINES ===================== */
#define I2C_BASE            I2C2
#define I2C_FLEXCOMM_ID     2
#define I2C_BAUDRATE        100000U  // 100 kHz for TCS34725

/* ===================== TCS34725 DEFINES ===================== */
#define TCS34725_ADDR       0x29
#define CMD_BIT             0x80

#define REG_ENABLE          0x00
#define REG_ID              0x12
#define REG_CDATAL          0x14

#define ENABLE_PON          0x01
#define ENABLE_AEN          0x02

/* ===================== I2C HELPER FUNCTIONS ===================== */
static status_t i2c_write(uint8_t reg, const uint8_t *data, size_t len)
{
    uint8_t buf[8];
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
    i2c_master_transfer_t xfer = {0};

    xfer.slaveAddress   = TCS34725_ADDR;
    xfer.direction      = kI2C_Read;
    xfer.subaddress     = CMD_BIT | reg;
    xfer.subaddressSize = 1;
    xfer.data           = data;
    xfer.dataSize       = len;
    xfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(I2C_BASE, &xfer);
}

/* ===================== MAIN ===================== */
int main(void)
{
    i2c_master_config_t cfg;
    uint32_t i2c_clk;
    uint8_t id;
    uint8_t enable;
    uint8_t raw[8];

    /* --- Safe boot init --- */
    BOARD_InitBootPins();      // sets pin mux only, safe
    BOARD_InitBootClocks();    // system clocks ready
    BOARD_InitDebugConsole();  // debug console ready

    PRINTF("\r\nUSB-IRIS-W1 TCS34725 RGBC Example (Safe Startup)\r\n");

    /* --- Flexcomm2 I2C setup (pins 16/17 on Arduino header J5) --- */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM2);          // attach SFRO 16 MHz to Flexcomm2
    CLOCK_EnableClock(kCLOCK_Flexcomm2);          // enable Flexcomm2 peripheral
    RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);   // reset Flexcomm2

    i2c_clk = CLOCK_GetFlexCommClkFreq(I2C_FLEXCOMM_ID);
    PRINTF("FLEXCOMM2 clock = %u Hz\r\n", i2c_clk);

    if (i2c_clk == 0)
    {
        PRINTF("ERROR: FLEXCOMM2 clock is 0!\r\n");
        while (1);
    }

    /* --- Init I2C master at 100 kHz --- */
    I2C_MasterGetDefaultConfig(&cfg);
    cfg.baudRate_Bps = I2C_BAUDRATE;
    I2C_MasterInit(I2C_BASE, &cfg, i2c_clk);

    /* --- Probe sensor --- */
    status_t s = i2c_read(REG_ID, &id, 1);
    PRINTF("TCS34725 ID = 0x%02X, status = %d\r\n", id, s);

    if (id != 0x44)
    {
        PRINTF("WARNING: Unexpected ID (expected 0x44)\r\n");
    }

    /* --- Power ON sensor --- */
    enable = ENABLE_PON;
    i2c_write(REG_ENABLE, &enable, 1);
    SDK_DelayAtLeastUs(3000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    enable = ENABLE_PON | ENABLE_AEN;
    i2c_write(REG_ENABLE, &enable, 1);

    PRINTF("TCS34725 enabled\r\n");

    /* --- Main loop: read RGBC values --- */
    while (1)
    {
        if (i2c_read(REG_CDATAL, raw, 8) == kStatus_Success)
        {
            uint16_t c = (raw[1] << 8) | raw[0];
            uint16_t r = (raw[3] << 8) | raw[2];
            uint16_t g = (raw[5] << 8) | raw[4];
            uint16_t b = (raw[7] << 8) | raw[6];

            PRINTF("C:%5d  R:%5d  G:%5d  B:%5d\r\n", c, r, g, b);
        }
        else
        {
            PRINTF("I2C read error\r\n");
        }

        SDK_DelayAtLeastUs(500000, CLOCK_GetFreq(kCLOCK_CoreSysClk)); // 500 ms
    }
}
