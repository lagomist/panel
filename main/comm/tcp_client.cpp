#include "tcp_client.h"
#include "socket_wrapper.h"

#include "esp_log.h"
#include <thread>


namespace TcpClient{


constexpr static char TAG[] = "tcp_client";
std::thread _thread;

static void tcp_client_task(void *pvParameters) {
    int res;
    SocketWrapper::Client client(SocketWrapper::Protocol::TCP);
    client.init(6666);
    while (1) {
        ESP_LOGI(TAG, "thread process.");
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    
    res = client.connect("192.168.4.1", 8888);
    if (res < 0) {
        ESP_LOGI(TAG,"Connection error");
        goto over;
    }

    // tcp_socket_info_t sock_info = {0};
    // sock_info.mark = instance_mark;           // 多客户端标识
    // sock_info.socket = tcp_socket;
    // /* allocation sock date buffer */
    // sock_info.data = (uint8_t *)malloc(DEFAULT_SOCK_BUF_SIZE + 8);
    // if (sock_info.data == NULL) {
    //     ESP_LOGE(TAG, "sock buffer malloc failed");
    //     goto error;
    // }

    // instance->socket = tcp_socket;
    // ESP_LOGI(TAG, "[-%d-] client started!", instance_mark);
    // if (server_conn_callback != NULL) {
    //     socket_connect_info_t conn_info;
    //     conn_info.socket = tcp_socket;
    //     conn_info.target_addr = NULL;
    //     conn_info.mark = sock_info.mark;
    //     server_conn_callback(conn_info);
    // }

    // while(1) {
    //     // Keep receiving until we have a reply
    //     int len = recv(tcp_socket, sock_info.data, DEFAULT_SOCK_BUF_SIZE, 0);
    //     if (len < 0) {
    //         ESP_LOGE(TAG, "Error occurred during recv");
    //         for (socket_client_list_t *instance = client_list_head; instance; instance = instance->next) {
    //             if (instance->mark == instance_mark) {
    //                 instance->socket = INVALID_SOCK;
    //                 break;
    //             }
    //         }
    //         xEventGroupSetBits(sock_event_group, TCP_CLIENT_RECOVER_BIT);
    //         break;
    //     }

    //     if(len > 0) {
    //         sock_info.data[len] = '\0';
    //         sock_info.len = len;
    //         if (tcp_client_recv_callback != NULL) {
    //             tcp_client_recv_callback(sock_info);
    //         }
    //     }
    // }

over:
    // if (tcp_socket > 0) {
    //     close(tcp_socket);
    // }
    // ESP_LOGW(TAG, "[-%d-] client delete!", instance_mark);
    // vTaskDelete(NULL);
}


void init() {
    _thread = std::thread(tcp_client_task, nullptr);
}

}
