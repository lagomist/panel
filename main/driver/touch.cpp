#include "touch.h"
#include "esp_log.h"

namespace touch {

constexpr static const char *TAG = "touch";

constexpr static uint8_t I2C_GT911_ADDRESS          = 0x5D;
constexpr static uint8_t I2C_GT911_ADDRESS_BACKUP   = 0x14;

/* GT911 registers */
constexpr static uint16_t GT911_READ_KEY_REG    = 0x8093;
constexpr static uint16_t GT911_READ_XY_REG     = 0x814E;
constexpr static uint16_t GT911_CONFIG_REG      = 0x8047;
constexpr static uint16_t GT911_PRODUCT_ID_REG  = 0x8140;
constexpr static uint16_t GT911_ENTER_SLEEP     = 0x8040;




int GT911::write_register(uint16_t reg, uint8_t data) {
    uint8_t buf[3];
    *(uint16_t *)&buf[0] = reg;
    buf[2] = data;
    return _device.write(buf, sizeof(buf));
}

int GT911::read_register(uint16_t reg, uint8_t buf[], uint32_t len) {
    return _device.trans_recv((uint8_t *)&reg, sizeof(reg), buf, len);
}

int GT911::read_data() {
    uint8_t buf[41];
    uint8_t clear = 0;

    read_register(GT911_READ_XY_REG, buf, 1);

    /* Any touch data? */
    if ((buf[0] & 0x80) == 0x00) {
        write_register(GT911_READ_XY_REG, clear);
    } else if ((buf[0] & 0x10) == 0x10) {
        /* Read all keys */
        uint8_t key_max = 1;
        read_register(GT911_READ_KEY_REG, buf, key_max);

        /* Clear all */
        write_register(GT911_READ_XY_REG, clear);

        /* Buttons status */
        _button = buf[0] ? 1 : 0;

    } else if ((buf[0] & 0x80) == 0x80) {
        /* Count of touched points */
        uint8_t touch_cnt = buf[0] & 0x0f;
        if (touch_cnt > 5 || touch_cnt == 0) {
            write_register(GT911_READ_XY_REG, clear);
            return 0;
        }

        /* Read all points */
        read_register(GT911_READ_XY_REG + 1, &buf[1], touch_cnt * 8);

        /* Clear all */
        write_register(GT911_READ_XY_REG, clear);

        /* Number of touched points */
        touch_cnt = (touch_cnt > GT911_TOUCH_MAX_POINTS ? GT911_TOUCH_MAX_POINTS : touch_cnt);
        _points = touch_cnt;

        /* Fill all coordinates */
        for (int i = 0; i < touch_cnt; i++) {
            _coords[i].x = ((uint16_t)buf[(i * 8) + 3] << 8) + buf[(i * 8) + 2];
            _coords[i].y = (((uint16_t)buf[(i * 8) + 5] << 8) + buf[(i * 8) + 4]);
            _coords[i].strength = (((uint16_t)buf[(i * 8) + 7] << 8) + buf[(i * 8) + 6]);
        }
    }

    return 0;
}

int GT911::get_coordinates(uint16_t *x, uint16_t *y, uint16_t *strength, uint8_t max_point_num) {
    /* Count of points */
    uint8_t point_num = (_points > max_point_num ? max_point_num : _points);

    for (int i = 0; i < point_num; i++) {
        x[i] = _coords[i].x;
        y[i] = _coords[i].y;

        if (strength) {
            strength[i] = _coords[i].strength;
        }
    }

    /* Invalidate */
    _points = 0;

    return point_num;
}

int GT911::get_button_state() {
    return _button;
}

void GT911::reset() {
    // if (tp->config.rst_gpio_num != GPIO_NUM_NC) {
    //     ESP_RETURN_ON_ERROR(gpio_set_level(tp->config.rst_gpio_num, tp->config.levels.reset), TAG, "GPIO set level error!");
    //     vTaskDelay(pdMS_TO_TICKS(10));
    //     ESP_RETURN_ON_ERROR(gpio_set_level(tp->config.rst_gpio_num, !tp->config.levels.reset), TAG, "GPIO set level error!");
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }
}

int GT911::init(uint32_t x_max, uint32_t y_max) {
    uint8_t buf[4] = {0};
    _x_max = x_max;
    _y_max = y_max;

    _device.init(I2C_GT911_ADDRESS, 400000);
    reset();

    read_register(GT911_PRODUCT_ID_REG, buf, 3);
    read_register(GT911_CONFIG_REG, &buf[3], 1);

    ESP_LOGI(TAG, "TouchPad_ID:0x%02x,0x%02x,0x%02x", buf[0], buf[1], buf[2]);
    ESP_LOGI(TAG, "TouchPad_Config_Version:%d", buf[3]);

    return 0;
}

} // namespace touch
