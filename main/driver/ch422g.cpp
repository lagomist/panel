#include "ch422g.h"


constexpr static uint8_t CH422G_IO_COUNT    = 8;

/* Register address */
constexpr static uint8_t CH422G_REG_WR_SET  = 0x24;
constexpr static uint8_t CH422G_REG_RD_IO   = 0x26;
constexpr static uint8_t CH422G_REG_WR_IO   = 0x38;
constexpr static uint8_t CH422G_REG_WR_OC   = 0x23;

/* Default register value on power-up */
constexpr static uint8_t DIR_REG_DEFAULT_VAL     = 0xff;
constexpr static uint8_t OUT_REG_DEFAULT_VAL     = 0xdf;
constexpr static uint8_t OD_OUT_REG_DEFAULT_VAL  = 0x03;

constexpr static const char *TAG = "ch422g";

CH422G::CH422G(uint8_t host) :
_setting_reg(host), _input_reg(host), _output_reg(host) {
    _setting_reg.init(CH422G_REG_WR_SET);
    _input_reg.init(CH422G_REG_RD_IO);
    _output_reg.init(CH422G_REG_WR_IO);
}

int CH422G::set_direction(Direct dir) {
    uint8_t cfg = dir;
    return _setting_reg.write(cfg);
}

uint8_t CH422G::read_input_reg() {
    uint8_t value = 0;
    int res = _input_reg.read(&value, 1);
    if (res == 0)
        return value;
    return 0;
}

int CH422G::write_output_reg(uint8_t value) {
    _reg_val = value;
    return _output_reg.write(_reg_val);
}

int CH422G::set_output_bit(uint8_t pin, uint8_t level) {
    if (level) {
        _reg_val |= (1 << pin);
    } else {
        _reg_val &= ~(1 << pin);
    }
    return write_output_reg(_reg_val);
}

int CH422G::get_input_bit(uint8_t pin) {
    uint8_t val = read_input_reg();
    int res = val & (1 << pin) ? 1 : 0;
    return res;
}
