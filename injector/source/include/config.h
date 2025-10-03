#pragma once
#include <string>

namespace injector {
    namespace config {
        extern std::string process;
        extern std::string dll;
        extern bool created;
        bool load_config();
    }
}