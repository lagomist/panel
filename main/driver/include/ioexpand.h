#pragma once

#include "gpio_wrapper.h"

namespace ioexpand {

int init(uint8_t host);

class GPO : public Wrapper::GPOBase {
public:
	GPO(uint8_t pin, int8_t pullmode = -1); 
	void set(uint8_t level = 1);
	int get();
};

}
