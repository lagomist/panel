#pragma once

#include <stdint.h>

namespace hwdef {

constexpr uint32_t LCD_HOR_RES          = 800;
constexpr uint32_t LCD_VER_RES          = 480;
constexpr uint32_t LCD_NUM_FB           = 2;

constexpr int LCD_PIXEL_CLOCK_HZ        = (18 * 1000 * 1000);
constexpr int LCD_BK_LIGHT_ON_LEVEL     = 1;
constexpr int LCD_BK_LIGHT_OFF_LEVEL    = !LCD_BK_LIGHT_ON_LEVEL;
constexpr int LCD_PIN_NUM_BK_LIGHT      = -1;
constexpr int LCD_PIN_NUM_HSYNC         = 46;
constexpr int LCD_PIN_NUM_VSYNC         = 3;
constexpr int LCD_PIN_NUM_DE            = 5;
constexpr int LCD_PIN_NUM_PCLK          = 7;
constexpr int LCD_PIN_NUM_DATA0         = 14;       // B3
constexpr int LCD_PIN_NUM_DATA1         = 38;       // B4
constexpr int LCD_PIN_NUM_DATA2         = 18;       // B5
constexpr int LCD_PIN_NUM_DATA3         = 17;       // B6
constexpr int LCD_PIN_NUM_DATA4         = 10;       // B7
constexpr int LCD_PIN_NUM_DATA5         = 39;       // G2
constexpr int LCD_PIN_NUM_DATA6         = 0;        // G3
constexpr int LCD_PIN_NUM_DATA7         = 45;       // G4
constexpr int LCD_PIN_NUM_DATA8         = 48;       // G5
constexpr int LCD_PIN_NUM_DATA9         = 47;       // G6
constexpr int LCD_PIN_NUM_DATA10        = 21;       // G7
constexpr int LCD_PIN_NUM_DATA11        = 1;        // R3
constexpr int LCD_PIN_NUM_DATA12        = 2;        // R4
constexpr int LCD_PIN_NUM_DATA13        = 42;       // R5
constexpr int LCD_PIN_NUM_DATA14        = 41;       // R6
constexpr int LCD_PIN_NUM_DATA15        = 40;       // R7
constexpr int LCD_PIN_NUM_DISP_EN       = -1;

constexpr int INDEV_PIN_IRQ             = 4;

constexpr uint32_t I2C_MASTER_NUM       = 0;
constexpr int I2C_MASTER_SCL_IO         = 9;
constexpr int I2C_MASTER_SDA_IO         = 8;

/* ioexpand pin */
constexpr uint8_t CTP_RST_PIN  		= 1;
constexpr uint8_t LCD_BL_PIN  		= 2;
constexpr uint8_t LCD_RST_PIN  		= 3;
constexpr uint8_t SD_CS_PIN  		= 4;
constexpr uint8_t USB_SEL_PIN  		= 5;

/* sd card */
constexpr uint8_t SD_SPI_HOST  	    = 2;
constexpr uint8_t SD_M0SI_PIN  		= 11;
constexpr uint8_t SD_MISO_PIN  		= 13;
constexpr uint8_t SD_CLK_PIN  		= 12;

}