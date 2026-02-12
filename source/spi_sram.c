/*
 * sram.c
 *
 *  Created on: 10 Feb 2026
 *      Author: liren
 */

#include <spi_sram.h>
#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_io_mux.h"

/* ================= CONFIG ================= */

#define SPI_BASE            SPI1
#define SPI_CLK_FREQ        CLOCK_GetFlexCommClkFreq(1)

#define CS_PORT             0U
#define CS_PIN              10U

#define SRAM_CMD_WRITE      0x02
#define SRAM_CMD_READ       0x03

/* ================= PRIVATE ================= */

static void cs_low(void)
{
    GPIO_PinWrite(GPIO, CS_PORT, CS_PIN, 0);
}

static void cs_high(void)
{
    GPIO_PinWrite(GPIO, CS_PORT, CS_PIN, 1);
}

/* ================= INIT ================= */

void SPI_SRAM_Init(void)
{
    spi_master_config_t config;
    gpio_pin_config_t cs_config = {kGPIO_DigitalOutput, 1};

    /* Enable FLEXCOMM1 clock */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM1);

    /* SPI config */
    SPI_MasterGetDefaultConfig(&config);
    config.baudRate_Bps = 1000000;
    config.polarity = kSPI_ClockPolarityActiveHigh;   // Mode 0
    config.phase = kSPI_ClockPhaseFirstEdge;          // Mode 0
    config.direction = kSPI_MsbFirst;
    config.enableMaster = true;
    config.sselNum = kSPI_Ssel0;   // not used

    SPI_MasterInit(SPI_BASE, &config, SPI_CLK_FREQ);

    /* Manual CS */
    GPIO_PortInit(GPIO, CS_PORT);
    GPIO_PinInit(GPIO, CS_PORT, CS_PIN, &cs_config);
    IO_MUX_SetPinMux(IO_MUX_GPIO10);
}

/* ================= WRITE ================= */

bool SPI_SRAM_Write(uint32_t address, const uint8_t *data, uint32_t length)
{
    uint8_t tx[4 + length];

    tx[0] = SRAM_CMD_WRITE;
    tx[1] = (address >> 16) & 0xFF;
    tx[2] = (address >> 8) & 0xFF;
    tx[3] = address & 0xFF;

    for (uint32_t i = 0; i < length; i++)
        tx[4 + i] = data[i];

    spi_transfer_t xfer = {
        .txData = tx,
        .rxData = NULL,
        .dataSize = sizeof(tx),
        .configFlags = 0
    };

    cs_low();
    SDK_DelayAtLeastUs(2, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    status_t status = SPI_MasterTransferBlocking(SPI_BASE, &xfer);

    cs_high();

    return (status == kStatus_Success);
}

/* ================= READ ================= */

bool SPI_SRAM_Read(uint32_t address, uint8_t *data, uint32_t length)
{
    uint8_t tx[4 + length];
    uint8_t rx[4 + length];

    tx[0] = SRAM_CMD_READ;
    tx[1] = (address >> 16) & 0xFF;
    tx[2] = (address >> 8) & 0xFF;
    tx[3] = address & 0xFF;

    for (uint32_t i = 0; i < length; i++)
        tx[4 + i] = 0xFF;

    spi_transfer_t xfer = {
        .txData = tx,
        .rxData = rx,
        .dataSize = sizeof(tx),
        .configFlags = 0
    };

    cs_low();
    SDK_DelayAtLeastUs(2, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    status_t status = SPI_MasterTransferBlocking(SPI_BASE, &xfer);

    cs_high();

    if (status != kStatus_Success)
        return false;

    for (uint32_t i = 0; i < length; i++)
        data[i] = rx[4 + i];

    return true;
}
