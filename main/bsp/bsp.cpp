#include "bsp.h"
#include "hwdef.h"
#include "gpio_wrapper.h"
#include "i2c_wrapper.h"
#include "os_wrapper.h"

#include "esp_log.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include <cstring>


namespace bsp {

constexpr static const char TAG[] = "bsp";

std::shared_ptr<GT911> touch = nullptr;
std::shared_ptr<Wrapper::GPOBase> ctp_rst = nullptr;
std::shared_ptr<Wrapper::GPOBase> lcd_bl = nullptr;
std::shared_ptr<Wrapper::GPOBase> lcd_rst = nullptr;
std::shared_ptr<Wrapper::GPOBase> sd_cs = nullptr;
std::shared_ptr<Wrapper::GPOBase> usb_sel = nullptr;
esp_lcd_panel_handle_t panel = nullptr;


static int ioexpand_init() {
	Wrapper::I2C::init(hwdef::I2C_MASTER_NUM, hwdef::I2C_MASTER_SDA_IO, hwdef::I2C_MASTER_SCL_IO);
	ioexpand::init(hwdef::I2C_MASTER_NUM);
	ctp_rst.reset(new ioexpand::GPO(hwdef::CTP_RST_PIN, 0));
	lcd_bl.reset(new ioexpand::GPO(hwdef::LCD_BL_PIN, 1));
	lcd_rst.reset(new ioexpand::GPO(hwdef::LCD_RST_PIN, 1));
	sd_cs.reset(new ioexpand::GPO(hwdef::SD_CS_PIN, 1));
	usb_sel.reset(new ioexpand::GPO(hwdef::USB_SEL_PIN, 0));

	ESP_LOGI(TAG, "ioexpand initialized successfully");
	return 0;
}

static int touch_init() {
	ESP_LOGI(TAG, "Initialize touch controller");
	Wrapper::GPI touch_irq(hwdef::INDEV_PIN_IRQ, 0);
	touch.reset(new GT911(hwdef::I2C_MASTER_NUM, *ctp_rst));
	touch->init(hwdef::LCD_HOR_RES, hwdef::LCD_VER_RES);
	return 0;
}

/*Initialize display and the required peripherals.*/
static void disp_init(void) {
	ESP_LOGI(TAG, "Initialize RGB LCD panel");
	esp_lcd_rgb_panel_config_t panel_config;
	memset(&panel_config, 0, sizeof(panel_config));
	// RGB565 in parallel mode; thus 16bit in width
	panel_config.data_width = 16; 
	panel_config.psram_trans_align = 64;
	panel_config.num_fbs = hwdef::LCD_NUM_FB;
	panel_config.clk_src = LCD_CLK_SRC_DEFAULT;
	panel_config.disp_gpio_num = hwdef::LCD_PIN_NUM_DISP_EN;
	panel_config.pclk_gpio_num = hwdef::LCD_PIN_NUM_PCLK;
	panel_config.vsync_gpio_num = hwdef::LCD_PIN_NUM_VSYNC;
	panel_config.hsync_gpio_num = hwdef::LCD_PIN_NUM_HSYNC;
	panel_config.de_gpio_num = hwdef::LCD_PIN_NUM_DE;
	panel_config.data_gpio_nums[0] = hwdef::LCD_PIN_NUM_DATA0;
	panel_config.data_gpio_nums[1] = hwdef::LCD_PIN_NUM_DATA1;
	panel_config.data_gpio_nums[2] = hwdef::LCD_PIN_NUM_DATA2;
	panel_config.data_gpio_nums[3] = hwdef::LCD_PIN_NUM_DATA3;
	panel_config.data_gpio_nums[4] = hwdef::LCD_PIN_NUM_DATA4;
	panel_config.data_gpio_nums[5] = hwdef::LCD_PIN_NUM_DATA5;
	panel_config.data_gpio_nums[6] = hwdef::LCD_PIN_NUM_DATA6;
	panel_config.data_gpio_nums[7] = hwdef::LCD_PIN_NUM_DATA7;
	panel_config.data_gpio_nums[8] = hwdef::LCD_PIN_NUM_DATA8;
	panel_config.data_gpio_nums[9] = hwdef::LCD_PIN_NUM_DATA9;
	panel_config.data_gpio_nums[10] = hwdef::LCD_PIN_NUM_DATA10;
	panel_config.data_gpio_nums[11] = hwdef::LCD_PIN_NUM_DATA11;
	panel_config.data_gpio_nums[12] = hwdef::LCD_PIN_NUM_DATA12;
	panel_config.data_gpio_nums[13] = hwdef::LCD_PIN_NUM_DATA13;
	panel_config.data_gpio_nums[14] = hwdef::LCD_PIN_NUM_DATA14;
	panel_config.data_gpio_nums[15] = hwdef::LCD_PIN_NUM_DATA15;
	panel_config.timings.pclk_hz = hwdef::LCD_PIXEL_CLOCK_HZ;
	panel_config.timings.h_res = hwdef::LCD_HOR_RES;
	panel_config.timings.v_res = hwdef::LCD_VER_RES;
	// The following parameters should refer to LCD spec
	panel_config.timings.hsync_back_porch = 8;
	panel_config.timings.hsync_front_porch = 8;
	panel_config.timings.hsync_pulse_width = 4;
	panel_config.timings.vsync_back_porch = 16;
	panel_config.timings.vsync_front_porch = 16;
	panel_config.timings.vsync_pulse_width = 4;
	panel_config.timings.flags.pclk_active_neg = true;
	// allocate frame buffer in PSRAM
	panel_config.flags.fb_in_psram = true;
	ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel));
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel));
	ESP_ERROR_CHECK(esp_lcd_panel_init(panel));
}

void init() {
	ioexpand_init();
	disp_init();
	touch_init();
}

}
