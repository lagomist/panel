#include "shell_wrapper.h"
#include "fs_wrapper.h"
#include "nvs_wrapper.h"
#include "wifi_wrapper.h"
#include "os_wrapper.h"
#include "cmds.h"
#include "bsp.h"
#include "comm.h"
#include "lv_port_disp.h"
#include "lvgl.h"
#include "esp_log.h"


extern "C" void app_main(void) {
    Wrapper::NVS::init("nvs");
    Wrapper::FileSystem::mount();

    Wrapper::Shell::registerCallback(cmds::call);

    bsp::init();
    comm::init();

    gui::init();

    gui::mutex->lock();
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    gui::mutex->unlock();
}


