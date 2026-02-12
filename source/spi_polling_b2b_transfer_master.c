#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "spi_sram.h"

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("23LC1024 Test\r\n");

    SPI_SRAM_Init();

    uint8_t tx[4] = {0x11, 0x22, 0x33, 0x44};
    uint8_t rx[4];

    while (1)
    {
        SPI_SRAM_Write(0x000000, tx, 4);
        SPI_SRAM_Read(0x000000, rx, 4);

        PRINTF("Read: %02X %02X %02X %02X\r\n",
               rx[0], rx[1], rx[2], rx[3]);

        SDK_DelayAtLeastUs(500000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}
