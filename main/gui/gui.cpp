#include "gui.h"
#include "lv_port_disp.h"
#include "file_explorer.h"
#include "os_wrapper.h"
#include "taskdef.h"
#include "lvgl.h"

#include "esp_log.h"

namespace gui {

constexpr static const char TAG[] = "gui";
static Wrapper::OS::Task _thread;
static UiPage * _ui_page = nullptr;
static void gui_thread(void * arg)
{
    gui::file_explorer::init('D');
    _ui_page = gui::file_explorer::get_ui();
    _ui_page->show();
	while (1) {
		Wrapper::OS::delay(1000);
	}
	 
}

void init(void)
{
	gui::port::init();
	_thread.create(gui_thread, nullptr, taskdef::NAME_GUI, taskdef::SIZE_GUI, taskdef::PRIO_GUI);
}

} // namespace gui

