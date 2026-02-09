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

#include "i2c_handler.h"

/*******************************************************************************
 * TCS34725 definitions
 ******************************************************************************/
#define TCS34725_ADDR   0x29

#define CMD_BIT         0x80
#define AUTO_INC        0x20

#define REG_ENABLE      0x00
#define REG_ID          0x12
#define REG_CDATAL      0x14

#define ENABLE_PON      0x01
#define ENABLE_AEN      0x02

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    uint8_t id;
    uint8_t raw[8];

    /* ---- Safe startup ---- */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("\r\nTCS34725 I2C Example (FRDM-RW612)\r\n");

    /* ---- Init I2C handler ---- */
    I2C_Handler_Init();
    PRINTF("I2C initialized\r\n");

    /* ---- Read sensor ID ---- */
    if (I2C_Handler_Read(TCS34725_ADDR,
                         CMD_BIT | REG_ID,
                         &id,
                         1) != kStatus_Success)
    {
        PRINTF("ERROR: Failed to read sensor ID\r\n");
        while (1);
    }

    PRINTF("TCS34725 ID = 0x%02X\r\n", id);

    /* ---- Power ON ---- */
    I2C_Handler_WriteReg(TCS34725_ADDR,
                         CMD_BIT | REG_ENABLE,
                         ENABLE_PON);

    SDK_DelayAtLeastUs(3000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    /* ---- Enable RGBC ADC ---- */
    I2C_Handler_WriteReg(TCS34725_ADDR,
                         CMD_BIT | REG_ENABLE,
                         ENABLE_PON | ENABLE_AEN);

    PRINTF("TCS34725 enabled\r\n");

    /* ---- Main loop ---- */
    while (1)
    {
        if (I2C_Handler_Read(TCS34725_ADDR,
                             CMD_BIT | AUTO_INC | REG_CDATAL,
                             raw,
                             8) == kStatus_Success)
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
