#pragma once

#include "touch.h"
#include "ioexpand.h"
#include "esp_lcd_types.h"
#include <memory>

namespace bsp {

extern std::shared_ptr<touch::GT911> touch;
extern esp_lcd_panel_handle_t panel;

void init();

}
