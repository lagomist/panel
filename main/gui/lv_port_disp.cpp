/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "os_wrapper.h"
#include "timer_wrapper.h"
#include "taskdef.h"
#include "hwdef.h"
#include "bsp.h"
#include "lvgl.h"

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

namespace gui {

/*********************
 *      DEFINES
 *********************/

constexpr uint32_t LV_TICK_PERIOD_MS    = 1;
constexpr uint32_t LV_TASK_MAX_DELAY_MS = 500;
constexpr uint32_t LV_TASK_MIN_DELAY_MS = 10;


/**********************
 *  STATIC VARIABLES
 **********************/

constexpr static const char TAG[] = "lv_port_disp";
Wrapper::OS::RecursiveMutex * mutex = nullptr;
static Wrapper::OS::Task _thread;

/**
 * gui task periodic timer task
*/
static void gui_disp_task(void *pvParameter) {
	ESP_LOGI(TAG, "start GUI diplay task");
	uint32_t task_delay_ms = LV_TASK_MAX_DELAY_MS;
	lv_tick_set_cb(Wrapper::OS::tick);
	while (1) {
		/* Try to take the semaphore, call lvgl related function on success */
		mutex->lock();
		task_delay_ms = lv_task_handler();
		mutex->unlock();

		if (task_delay_ms > LV_TASK_MAX_DELAY_MS) {
            task_delay_ms = LV_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < LV_TASK_MIN_DELAY_MS) {
            task_delay_ms = LV_TASK_MIN_DELAY_MS;
        }
		Wrapper::OS::delay(task_delay_ms);
	}
}

static void touch_trigger_cb(lv_indev_t * drv, lv_indev_data_t * data) {
	if (bsp::touch == nullptr) return;
	uint16_t touchpad_x[1] = {0};
	uint16_t touchpad_y[1] = {0};
	uint8_t touchpad_cnt = 0;

	/* Read touch controller data */
	bsp::touch->read_data();

	/* Get coordinates */
	touchpad_cnt = bsp::touch->get_coordinates(touchpad_x, touchpad_y, nullptr, 1);

	if (touchpad_cnt > 0) {
		data->point.x = touchpad_x[0];
		data->point.y = touchpad_y[0];
		data->state = LV_INDEV_STATE_PR;
	} else {
		data->state = LV_INDEV_STATE_REL;
	}
}

static void disp_flush_cb(lv_display_t * drv, const lv_area_t * area, uint8_t * px_map) {
	if (bsp::panel == nullptr) return;
	int offsetx1 = area->x1;
	int offsetx2 = area->x2;
	int offsety1 = area->y1;
	int offsety2 = area->y2;
	// pass the draw buffer to the driver
	esp_lcd_panel_draw_bitmap(bsp::panel, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, px_map);
	lv_disp_flush_ready(drv);
}



/**********************
 *   GLOBAL FUNCTIONS
 **********************/


void init(void) {
	/*------------------------------------
	 * Create a display and set a flush_cb
	 * -----------------------------------*/
	ESP_LOGI(TAG, "Initialize LVGL library");
	void *buf1 = nullptr;
	void *buf2 = nullptr;
	lv_init();
	lv_display_t * disp = lv_display_create(hwdef::LCD_HOR_RES, hwdef::LCD_VER_RES);
	lv_display_set_flush_cb(disp, disp_flush_cb);
	ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(bsp::panel, hwdef::LCD_NUM_FB, &buf1, &buf2));
	// initialize LVGL draw buffers
	uint8_t pix_size = lv_color_format_get_size(lv_display_get_color_format(disp));
	size_t buf_size = hwdef::LCD_HOR_RES * hwdef::LCD_VER_RES * pix_size;
	lv_display_set_buffers(disp, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_DIRECT);

	/* Register a touchpad input device */
	lv_indev_t * indev_drv = lv_indev_create();
	lv_indev_set_type(indev_drv, LV_INDEV_TYPE_POINTER);
	lv_indev_set_read_cb(indev_drv, touch_trigger_cb);
	
	/* If you want to use a task to create the graphic, you NEED to create a Pinned task
	 * Otherwise there can be problem such as memory corruption and so on.
	 * NOTE: When not using Wi-Fi nor Bluetooth you can pin the gui_disp_task to core 0 */
	mutex = new Wrapper::OS::RecursiveMutex();
	_thread.create(gui_disp_task, nullptr, taskdef::NAME_LVGL, taskdef::SIZE_LVGL, taskdef::PRIO_LVGL, Wrapper::OS::Task::Core::CORE_1);
}

} // namespace gui

