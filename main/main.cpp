#include "shell.h"
#include "cmds.h"
#include "nvs_wrapper.h"
#include "wifi_wrapper.h"
#include "tcp_client.h"


extern "C" void app_main(void) {
    // Initialize NVS
    NvsWrapper::init("nvs");

    WifiWrapper::netif_init();
    WifiWrapper::Station::init();

    // Shell::registerCallback(cmds::call);

    TcpClient::init();

    // gui::init();
}


