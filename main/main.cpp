#include "shell_wrapper.h"
#include "fs_wrapper.h"
#include "nvs_wrapper.h"
#include "wifi_wrapper.h"
#include "os_wrapper.h"
#include "cmds.h"
#include "bsp.h"
#include "comm.h"
#include "sd_card.h"
#include "lv_port_disp.h"
#include "lvgl.h"
#include "esp_log.h"

static constexpr char TAG[] = "main" ;

static void file_explorer_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = static_cast<lv_obj_t *>(lv_event_get_target(e));

    if(code == LV_EVENT_VALUE_CHANGED) {
        const char * cur_path =  lv_file_explorer_get_current_path(obj);
        const char * sel_fn = lv_file_explorer_get_selected_file_name(obj);
        ESP_LOGI(TAG, "path:<%s>, fn:<%s>", cur_path, sel_fn);
    }
}


extern "C" void app_main(void) {
    Wrapper::NVS::init("nvs");
    Wrapper::FileSystem::mount();

    Wrapper::Shell::registerCallback(cmds::call);

    bsp::init();
    comm::init();

    gui::init();

    gui::mutex->lock();
    lv_obj_t * file_explorer = lv_file_explorer_create(lv_screen_active());
    if (file_explorer == nullptr) {
        ESP_LOGE("main", "create failed");
        return;
    }
    lv_file_explorer_set_sort(file_explorer, LV_EXPLORER_SORT_KIND);
    std::string path = "D:/";
    ESP_LOGW(TAG, "open dir: %s", path.data());
    lv_file_explorer_open_dir(file_explorer, path.data());
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_FS_DIR, path.data());
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_HOME_DIR, path.data());
    std::string viode_path = path + "Videos";
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_VIDEO_DIR, viode_path.data());
    std::string picture_path = path + "Pictures";
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_PICTURES_DIR, picture_path.data());
    lv_obj_add_event_cb(file_explorer, file_explorer_event_handler, LV_EVENT_ALL, NULL);
    gui::mutex->unlock();
}


