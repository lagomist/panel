#include "comm.h"
#include "tcp_client.h"
#include "tcp_data_handle.h"
#include "app_cfg.h"

#include "wifi_wrapper.h"
#include "shell_wrapper.h"
#include "json_wrapper.h"
#include "ble_wrapper.h"

#include "esp_log.h"


namespace comm {

static const char TAG[] = "comm";
static OBuf _buf;

static void wifi_conn_cb() {
    // start tcp client task
    TcpClient::start();
}

static void ble_recv_cb(IBuf recv) {
    ESP_LOGI(TAG, "recv: %.*s", recv.size(), recv.data());
}


void init() {
    Wrapper::WiFi::netif_init();
    Wrapper::WiFi::Station::init();
    Wrapper::WiFi::Station::set_connect_cb(wifi_conn_cb);

    Wrapper::BLE::DefaultServer::init(Cfg::DEVICE_MARK);
    Wrapper::BLE::DefaultServer::registerRecvCallback(ble_recv_cb);

    TcpDataHandle::init(); 
}

}
