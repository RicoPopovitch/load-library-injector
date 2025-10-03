#include "config.h"
#include "process.h"
#include "inject.h"
#include <iostream>

int main() {
    
    if (!injector::process::get_process(injector::config::process)) {
        return 1;
    }

    if (!injector::process::open_process()) {
        return 1;
    }

    if (!injector::inject::inject(injector::config::get_dll_path())) {
        return 1;
    }

    system("pause");
    return 0;
}