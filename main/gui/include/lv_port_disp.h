#pragma once

#include "os_wrapper.h"

namespace gui {

class UiPage {
public:
    using entry = void(*)(void *arg);
    UiPage(const char* name, entry show, entry hide): _name(name), _show(show), _hide(hide) {}
    ~UiPage() {}
    void show(void *arg = nullptr) { _show(arg); }
    void hide(void *arg = nullptr) { _hide(arg); }
private:
    const char *_name;
    entry _show;
    entry _hide;
};

namespace port {

extern Wrapper::OS::RecursiveMutex * mutex;

void init(void);

}

}
