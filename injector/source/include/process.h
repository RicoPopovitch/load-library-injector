#pragma once
#include <windows.h>
#include <string>

namespace injector {
    namespace process {
        extern HANDLE handle;
        extern DWORD procid;
        
        bool get_process(const std::string& process);
        bool open_process();
    }
}
