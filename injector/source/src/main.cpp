#include "config.h"
#include "process.h"
#include "inject.h"
#include <windows.h>
#include <iostream>

int main() {
    FreeConsole();
    if (!injector::config::load_config()) {
        if (injector::config::created) {
            return 1;
        }
        MessageBoxA(NULL, "failed to read config, check for typos", "error", MB_OK | MB_ICONINFORMATION);
        return 1;
    }
    
    if (!injector::process::get_process(injector::config::process)) {
        MessageBoxA(NULL, "failed to find process, check for typos", "error", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    if (!injector::process::open_process()) {
        MessageBoxA(NULL, "failed to open process, check for typos", "error", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    if (!injector::inject::inject(injector::config::dll)) {
        MessageBoxA(NULL, "injection failed", "error", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    return 0;
}