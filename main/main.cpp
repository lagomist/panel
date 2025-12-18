#include "shell_wrapper.h"
#include "fs_wrapper.h"
#include "nvs_wrapper.h"
#include "wifi_wrapper.h"
#include "os_wrapper.h"
#include "cmds.h"
#include "bsp.h"
#include "comm.h"
#include "sd_card.h"
#include "gui.h"
#include "esp_log.h"


extern "C" void app_main(void) {
    Wrapper::NVS::init("nvs");
    Wrapper::FileSystem::mount();

    Wrapper::Shell::registerCallback(cmds::call);

    bsp::init();
    comm::init();

    gui::init();
}


