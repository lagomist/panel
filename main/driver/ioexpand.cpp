#include "ioexpand.h"
#include "ch422g.h"
#include "hwdef.h"

namespace ioexpand {

static CH422G * _dev = nullptr;

int init(uint8_t host) {
    _dev = new CH422G(host);
    _dev->set_direction(CH422G::Direct::OUT);
    return 0;
}

GPO::GPO(uint8_t pin, int8_t pullmode) : Wrapper::GPOBase(pin) {
    if (pullmode != -1 && _dev != nullptr) {
        set(pullmode);
    }
}

void GPO::set(uint8_t level) {
    _dev->set_output_bit(_pin, level);
}

int GPO::get() {
    return _dev->get_input_bit(_pin);
}


}

