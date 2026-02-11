/*
 * sram.c
 *
 *  Created on: 10 Feb 2026
 *      Author: liren
 */

#include "sram.h"
#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_reset.h"

/* ================= USER CONFIG ================= */

/* FLEXCOMM1 */
#define SRAM_SPI              SPI1
#define SRAM_FC_INDEX         1

/* CS pin: GPIO port 1 pin 9 (change if needed) */
#define SRAM_CS_GPIO          GPIO
#define SRAM_CS_PORT          1
#define SRAM_CS_PIN           9

/* ================= 23LC1024 ================= */

#define CMD_READ   0x03
#define CMD_WRITE  0x02
#define CMD_WREN   0x06
#define CMD_WRMR   0x01
#define MODE_SEQ   0x40

/* ============================================== */

static inline void cs_low(void)
{
    GPIO_PinWrite(SRAM_CS_GPIO, SRAM_CS_PORT, SRAM_CS_PIN, 0);
}

static inline void cs_high(void)
{
    GPIO_PinWrite(SRAM_CS_GPIO, SRAM_CS_PORT, SRAM_CS_PIN, 1);
}

static status_t spi_xfer(uint8_t *tx, uint8_t *rx, size_t len)
{
    spi_transfer_t xfer = {0};

    xfer.txData = tx;
    xfer.rxData = rx;
    xfer.dataSize = len;
    xfer.configFlags = kSPI_FrameAssert; /* manual CS */

    return SPI_MasterTransferBlocking(SRAM_SPI, &xfer);
}

static status_t write_enable(void)
{
    uint8_t cmd = CMD_WREN;
    cs_low();
    status_t st = spi_xfer(&cmd, NULL, 1);
    cs_high();
    return st;
}

static status_t set_sequential_mode(void)
{
    uint8_t cmd[2] = { CMD_WRMR, MODE_SEQ };

    write_enable();

    cs_low();
    status_t st = spi_xfer(cmd, NULL, 2);
    cs_high();

    return st;
}

status_t sram23_init(void)
{
    spi_master_config_t spi_cfg;
    gpio_pin_config_t cs_cfg =
    {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1
    };

    /* 1) Attach clock to FC1 */
    CLOCK_AttachClk(kFFRO_to_FLEXCOMM1);

    /* 2) Enable FC1 clock */
    CLOCK_EnableClock(kCLOCK_Flexcomm1);

    /* 3) Reset FC1 so SPI owns it */
    RESET_PeripheralReset(kFC1_RST_SHIFT_RSTn);

    uint32_t fc_clk = CLOCK_GetFlexCommClkFreq(SRAM_FC_INDEX);
    if (fc_clk == 0)
    {
        while (1); /* clock configuration error */
    }

    SPI_MasterGetDefaultConfig(&spi_cfg);
    spi_cfg.baudRate_Bps = 1000000U;
    spi_cfg.direction = kSPI_MsbFirst;

    SPI_MasterInit(SRAM_SPI, &spi_cfg, fc_clk);

    GPIO_PinInit(SRAM_CS_GPIO, SRAM_CS_PORT, SRAM_CS_PIN, &cs_cfg);
    cs_high();

    /* REQUIRED for 23LC1024 */
    return set_sequential_mode();
}

status_t sram23_write(uint32_t addr, const uint8_t *buf, size_t len)
{
    if ((addr + len) > SRAM_23LC1024_SIZE_BYTES)
        return kStatus_OutOfRange;

    uint8_t cmd[4] =
    {
        CMD_WRITE,
        (addr >> 16) & 0xFF,
        (addr >> 8)  & 0xFF,
        addr & 0xFF
    };

    write_enable();

    cs_low();
    spi_xfer(cmd, NULL, 4);
    spi_xfer((uint8_t *)buf, NULL, len);
    cs_high();

    return kStatus_Success;
}

status_t sram23_read(uint32_t addr, uint8_t *buf, size_t len)
{
    if ((addr + len) > SRAM_23LC1024_SIZE_BYTES)
        return kStatus_OutOfRange;

    uint8_t cmd[4] =
    {
        CMD_READ,
        (addr >> 16) & 0xFF,
        (addr >> 8)  & 0xFF,
        addr & 0xFF
    };

    cs_low();
    spi_xfer(cmd, NULL, 4);
    spi_xfer(NULL, buf, len);
    cs_high();

    return kStatus_Success;
}
