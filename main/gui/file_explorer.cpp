#include "file_explorer.h"
#include "gui.h"
#include "lv_port_disp.h"
#include "os_wrapper.h"
#include "taskdef.h"
#include "lvgl.h"

#include "esp_log.h"

namespace gui {


namespace file_explorer { 

constexpr static const char TAG[] = "gui::file_explorer";
static UiPage * page_file_explorer = nullptr;
static lv_obj_t * _file_explorer = nullptr;
static char _letter = 'D';


static void file_explorer_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = static_cast<lv_obj_t *>(lv_event_get_target(e));

    if (code != LV_EVENT_VALUE_CHANGED) return;
    
    const char * cur_path =  lv_file_explorer_get_current_path(obj);
    const char * sel_fn = lv_file_explorer_get_selected_file_name(obj);
    ESP_LOGI(TAG, "path:<%s>, fn:<%s>", cur_path, sel_fn);
}

static void file_explorer_init()
{
	Wrapper::OS::LockGuard guard(*gui::port::mutex);
    if (_file_explorer != nullptr) return;

    _file_explorer = lv_file_explorer_create(lv_screen_active());
    lv_file_explorer_set_sort(_file_explorer, LV_EXPLORER_SORT_KIND);
    const char path[4] = {_letter, ':', '/'};
    ESP_LOGI(TAG, "open path:<%s>", path);
    lv_file_explorer_open_dir(_file_explorer, path);
    lv_file_explorer_set_quick_access_path(_file_explorer, LV_EXPLORER_FS_DIR, path);
    lv_file_explorer_set_quick_access_path(_file_explorer, LV_EXPLORER_HOME_DIR, path);
    // For video path
    std::string video_path = std::string(path) + "Videos";
    lv_file_explorer_set_quick_access_path(_file_explorer, LV_EXPLORER_VIDEO_DIR, video_path.c_str());

    // For picture path  
    std::string picture_path = std::string(path) + "Pictures";
    lv_file_explorer_set_quick_access_path(_file_explorer, LV_EXPLORER_PICTURES_DIR, picture_path.c_str());

    lv_obj_add_event_cb(_file_explorer, file_explorer_event_handler, LV_EVENT_ALL, NULL);
}

static void ui_file_explorer_show(void *arg)
{
	Wrapper::OS::LockGuard guard(*gui::port::mutex);
    file_explorer_init();
    if (_file_explorer == nullptr) return;
    lv_obj_remove_flag(_file_explorer, LV_OBJ_FLAG_HIDDEN);
}

static void ui_file_explorer_hide(void *arg)
{
	Wrapper::OS::LockGuard guard(*gui::port::mutex);
    if (_file_explorer == nullptr) return;
    lv_obj_add_flag(_file_explorer, LV_OBJ_FLAG_HIDDEN);
}


UiPage * get_ui() {
    return page_file_explorer;
}

void init(char letter)
{
    _letter = letter;
    page_file_explorer = new UiPage("file_explorer", ui_file_explorer_show, ui_file_explorer_hide);
}

}

} // namespace gui
