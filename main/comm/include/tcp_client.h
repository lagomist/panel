#pragma once

#include "bufdef.h"
#include <string_view>

namespace TcpClient {

using RecvCallback = void (*)(IBuf);
using ConnCallback = void (*)();

void register_connect_cb(ConnCallback cb);
void register_recv_cb(RecvCallback cb);

int send(IBuf buf);

void start();
void init();

}
