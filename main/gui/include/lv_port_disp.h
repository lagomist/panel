#pragma once

#include "os_wrapper.h"

namespace gui {

extern Wrapper::OS::RecursiveMutex * mutex;

void init(void);

}
