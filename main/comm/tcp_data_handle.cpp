#include "tcp_data_handle.h"
#include "tcp_client.h"
#include "socket_wrapper.h"
#include "json_wrapper.h"
#include "shell_wrapper.h"
#include "utils_wrapper.h"
#include "app_cfg.h"

#include "esp_log.h"
#include "esp_heap_caps.h"
#include <cstring>

namespace TcpDataHandle {

static const char TAG[] = "TcpDataHandle";
static uint8_t* _tx_buffer = nullptr;
static OBuf _rx_buffer;

FrameHeader frameUnpack(IBuf& buf, OBuf& out) {
    FrameHeader frame;
    memset(&frame, 0, sizeof(frame));

    /* data frame parse*/
    frame = *(FrameHeader *)buf.data();
    if (frame.head != TcpDataHandle::FRAME_HEAD || frame.length != (buf.size() - sizeof(FrameHeader))) {
        out.clear();
        frame.type = FrameType::UNKNOWN;
        return frame;
    }

    out = buf.substr(sizeof(FrameHeader));
    return frame;
}

int packageSend(uint8_t goal, FrameType type, IBuf buf) {
    _tx_buffer[0] = TcpDataHandle::FRAME_HEAD;
    _tx_buffer[1] = type;
    _tx_buffer[2] = goal;
    _tx_buffer[3] = 1;
    *(uint16_t *)&_tx_buffer[4] = buf.size();
    memcpy(&_tx_buffer[6], buf.data(), buf.size());

    return TcpClient::send({_tx_buffer, buf.size() + sizeof(FrameHeader)});
}

OBuf json_string_parse(IBuf buf) {
    std::string json_str = (char *)buf.data();
    Wrapper::JsonObject json(json_str);
    OBuf out;
    if (!json.isObject()) {
        ESP_LOGE(TAG, "json parse failed.");
        return out;
    }

    if (json["cmd"].isString()) {
        out = Wrapper::Utils::snprint("%s", json["cmd"].getString().data());
    }
    if (json["args"].isArray()) {
        for (int i = 0; i < json["args"].getArraySize(); i++) {
            out += Wrapper::Utils::snprint(" %s", json["args"][i].getString().data());
        }
    }

    return out;
}

static void tcp_conn_cb() {
    /*  注册身份  */
    Wrapper::JsonObject json;
    Wrapper::JsonObject args;
    args.setToArray();
    json.add(TcpDataHandle::JSON_KEY_CMD, "login");
    args.addToArray(Cfg::DEVICE_MARK);
    json.addToObject(TcpDataHandle::JSON_KEY_ARGS, args);

    IBuf data = (uint8_t *)json.serialize().data();
    ESP_LOGI(TAG, "send: %.*s", data.size(), data.data());
    int res = TcpDataHandle::packageSend(1, TcpDataHandle::FrameType::JSON, data);
    if (res < 0) {
        ESP_LOGE(TAG, "Error occurred during packageSend");
    }
}

static void tcp_recv_cb(IBuf recv) {
    TcpDataHandle::FrameHeader frame = TcpDataHandle::frameUnpack(recv, _rx_buffer);
    if (frame.type == FrameType::UNKNOWN) {
        ESP_LOGE(TAG, "unknown frame.");
        return;
    }
}

IBuf recv(int timeout_ms) {
    // if ()
    return _rx_buffer;
}


int init() {
    /* allocation sock date buffer */
    _tx_buffer = (uint8_t *)heap_caps_malloc(Cfg::SOCK_BUF_SIZE + sizeof(FrameHeader), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (_tx_buffer == nullptr) {
        ESP_LOGE(TAG, "tx buffer malloc failed");
        return -1;
    }

    TcpClient::init();
    TcpClient::register_recv_cb(tcp_recv_cb);
    TcpClient::register_connect_cb(tcp_conn_cb);

    return 0;
}

}
