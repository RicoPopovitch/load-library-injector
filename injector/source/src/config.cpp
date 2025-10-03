#include "config.h"
#include <windows.h>
#include <fstream>
#include <iostream>

namespace injector {
    namespace config {
        std::string process;
        std::string dll;
        bool created = false;
        
        std::string get_dir() {
            char exe_path[MAX_PATH];
            GetModuleFileNameA(NULL, exe_path, MAX_PATH);
            std::string path(exe_path);
            size_t pos = path.find_last_of("\\/");
            return path.substr(0, pos + 1);
        }
        
        bool load_config() {
            std::string config_path = get_dir() + "config.txt";
            std::ifstream file(config_path);
            
            if (!file.is_open()) {
                std::ofstream create_file(config_path);
                create_file << "process=notepad.exe\n";
                create_file << "dll=test.dll\n";
                create_file.close();
                created = true;
                MessageBoxA(NULL, "please edit the config.txt file then restart", "config created", MB_OK | MB_ICONINFORMATION);
                return false;
            }

            std::string line;
            while (std::getline(file, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    
                    if (key == "process") {
                        process = value;
                    } else if (key == "dll") {
                        dll = get_dir() + value;
                    }
                }
            }
            
            return !process.empty() && !dll.empty();
        }
    }
}
