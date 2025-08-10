#pragma once

#include "cstdint"
#include <string_view>

namespace sd_card {

std::string_view get_base_path();

int mount(uint8_t spi_port, int spi_cs);
int unmount();
int format();

}
