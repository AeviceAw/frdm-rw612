#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_io_mux.h"
#include "clock_config.h"
#include <string.h>

#define SPI_MASTER          SPI1
#define SPI_CLOCK_FREQ      CLOCK_GetFlexCommClkFreq(1)

#define CS_PORT             0U
#define CS_PIN              10U

#define SRAM_CMD_WRITE      0x02
#define SRAM_CMD_READ       0x03

static void SPI_Init(void);
static void CS_Init(void);
static void SRAM_Write(uint32_t addr, uint8_t *data, uint32_t len);
static void SRAM_Read(uint32_t addr, uint8_t *data, uint32_t len);

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("\r\n==== 23LC1024 SPI Test ====\r\n");

    SPI_Init();
    CS_Init();

    uint8_t txData[4] = {0x11, 0x22, 0x33, 0x44};
    uint8_t rxData[4];

    while (1)
    {
        SRAM_Write(0x000000, txData, 4);
        SRAM_Read(0x000000, rxData, 4);

        PRINTF("Read: %02X %02X %02X %02X\r\n",
               rxData[0], rxData[1], rxData[2], rxData[3]);

        SDK_DelayAtLeastUs(1000000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}

static void SPI_Init(void)
{
    spi_master_config_t config;

    CLOCK_AttachClk(kSFRO_to_FLEXCOMM1);

    SPI_MasterGetDefaultConfig(&config);

    config.baudRate_Bps = 1000000U;
    config.polarity = kSPI_ClockPolarityActiveHigh;      // Mode 0
    config.phase = kSPI_ClockPhaseFirstEdge;             // Mode 0
    config.direction = kSPI_MsbFirst;
    config.enableMaster = true;
    config.sselNum = kSPI_Ssel0;     // Not used (manual CS)

    SPI_MasterInit(SPI_MASTER, &config, SPI_CLOCK_FREQ);

    PRINTF("SPI Initialized\r\n");
}

static void CS_Init(void)
{
    gpio_pin_config_t cs_config = {
        kGPIO_DigitalOutput,
        1U,   // default HIGH
    };

    GPIO_PortInit(GPIO, CS_PORT);
    GPIO_PinInit(GPIO, CS_PORT, CS_PIN, &cs_config);

    IO_MUX_SetPinMux(IO_MUX_GPIO10);

    PRINTF("Manual CS Ready (GPIO10)\r\n");
}

static void SRAM_Write(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t tx[4 + len];

    tx[0] = SRAM_CMD_WRITE;
    tx[1] = (addr >> 16) & 0xFF;
    tx[2] = (addr >> 8) & 0xFF;
    tx[3] = addr & 0xFF;

    for (uint32_t i = 0; i < len; i++)
        tx[4 + i] = data[i];

    spi_transfer_t xfer = {
        .txData = tx,
        .rxData = NULL,
        .dataSize = sizeof(tx),
        .configFlags = kSPI_FrameAssert
    };

    GPIO_PinWrite(GPIO, CS_PORT, CS_PIN, 0);
    SDK_DelayAtLeastUs(2, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    SPI_MasterTransferBlocking(SPI_MASTER, &xfer);

    GPIO_PinWrite(GPIO, CS_PORT, CS_PIN, 1);
}

static void SRAM_Read(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t tx[4 + len];
    uint8_t rx[4 + len];

    tx[0] = SRAM_CMD_READ;
    tx[1] = (addr >> 16) & 0xFF;
    tx[2] = (addr >> 8) & 0xFF;
    tx[3] = addr & 0xFF;

    for (uint32_t i = 0; i < len; i++)
        tx[4 + i] = 0xFF;

    spi_transfer_t xfer = {
        .txData = tx,
        .rxData = rx,
        .dataSize = sizeof(tx),
        .configFlags = kSPI_FrameAssert
    };

    GPIO_PinWrite(GPIO, CS_PORT, CS_PIN, 0);
    SDK_DelayAtLeastUs(2, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    SPI_MasterTransferBlocking(SPI_MASTER, &xfer);

    GPIO_PinWrite(GPIO, CS_PORT, CS_PIN, 1);

    for (uint32_t i = 0; i < len; i++)
        data[i] = rx[4 + i];
}
