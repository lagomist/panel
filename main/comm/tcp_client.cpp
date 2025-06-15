#include "tcp_client.h"
#include "socket_wrapper.h"
#include "app_cfg.h"
#include "taskdef.h"
#include "os_wrapper.h"

#include "esp_log.h"


namespace TcpClient {


constexpr static char TAG[] = "TcpClient";
static Wrapper::Socket::Client* _client = nullptr;
static Wrapper::OS::Task _thread;;
static RecvCallback _recv_cb = nullptr;
static ConnCallback _connect_cb = nullptr;


static void tcp_client_task(void *pvParameters) {
    int res = 0;
    uint8_t* rx_buf = nullptr;
    _client->init(Cfg::CLIENT_BIND_PORT);
    
    res = _client->connect(Cfg::SOFTAP_SERVER_IP, Cfg::SOFTAP_SERVER_PORT);
    if (res < 0) {
        ESP_LOGI(TAG,"Connection error");
        goto over;
    }

    /* allocation sock date buffer */
    rx_buf = (uint8_t *)malloc(Cfg::SOCK_BUF_SIZE + 8);
    if (rx_buf == nullptr) {
        ESP_LOGE(TAG, "buffer malloc failed");
        goto over;
    }

    // connect callback
    if (_connect_cb) {
        ESP_LOGI(TAG, "TCP server connected.");
        _connect_cb();
    }

    while(1) {
        // Keep receiving until we have a reply
        int len = _client->recv(rx_buf, Cfg::SOCK_BUF_SIZE + 8);
        if (len < 0) {
            ESP_LOGE(TAG, "Error occurred during recv");
            break;
        }

        if (_recv_cb) {
            _recv_cb({rx_buf, (unsigned int )len});
        }
    }

over:
    ESP_LOGW(TAG, "client delete!");
}


void register_connect_cb(ConnCallback cb) {
    _connect_cb = cb;
}

void register_recv_cb(RecvCallback cb) {
    _recv_cb = cb;
}

int send(IBuf buf) {
    return _client->send(buf.data(), buf.size());
}

void start() {
    if (_thread.is_inited()) {
        _thread.del();
    }
    _thread.create(tcp_client_task, nullptr, taskdef::NAME_TCP, taskdef::SIZE_TCP, taskdef::PRIO_TCP);
}

void init() {
    _client = new Wrapper::Socket::Client(Wrapper::Socket::Protocol::TCP);
}

}
