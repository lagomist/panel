#pragma once

namespace taskdef {

constexpr const char * NAME_TCP         = "tcp_client";
constexpr const size_t PRIO_TCP         = 10;
constexpr const size_t SIZE_TCP         = 4096;

constexpr const char * NAME_LVGL        = "lvgl";
constexpr const size_t PRIO_LVGL        = 2;
constexpr const size_t SIZE_LVGL        = 12 * 1024;

constexpr const char * NAME_GUI        = "gui";
constexpr const size_t PRIO_GUI        = 5;
constexpr const size_t SIZE_GUI        = 10 * 1024;

}
