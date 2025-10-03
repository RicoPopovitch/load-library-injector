#pragma once
#include <string>
#include <windows.h>

namespace injector {
    namespace config {
        const std::string process = "notepad.exe";
        
        inline std::string get_dll_path() {
            char exe_path[MAX_PATH];
            GetModuleFileNameA(NULL, exe_path, MAX_PATH);
            std::string path(exe_path);
            size_t pos = path.find_last_of("\\/");
            return path.substr(0, pos + 1) + "test.dll";
        }
    }
}