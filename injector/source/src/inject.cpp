#include "inject.h"
#include "process.h"
#include <windows.h>
#include <iostream>
#include <psapi.h>

namespace injector {
    namespace inject {
        bool is_loaded(const std::string& name) {
            HMODULE modules[1024];
            DWORD needed;
            
            if (EnumProcessModules(process::handle, modules, sizeof(modules), &needed)) {
                for (DWORD i = 0; i < (needed / sizeof(HMODULE)); i++) {
                    char module[MAX_PATH];
                    if (GetModuleBaseNameA(process::handle, modules[i], module, sizeof(module))) {
                        if (name == module) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        
        bool inject(const std::string& dll) {
            std::string name = dll;
            size_t pos = name.find_last_of("\\/");
            if (pos != std::string::npos) {
                name = name.substr(pos + 1);
            }
            
            if (is_loaded(name)) {
                MessageBoxA(NULL, "dll already loaded...", "error", MB_OK | MB_ICONINFORMATION);
                return true;
            }

                LPVOID mem = VirtualAllocEx(
                    process::handle,
                    nullptr,
                    dll.length() + 1,
                    MEM_COMMIT | MEM_RESERVE,
                    PAGE_READWRITE
                );
        
                if (!mem) {
                    return false;
                }
        
                SIZE_T bytes;
                if (!WriteProcessMemory(
                    process::handle,
                    mem,
                    dll.c_str(),
                    dll.length() + 1,
                    &bytes
                )) {
                    VirtualFreeEx(process::handle, mem, 0, MEM_RELEASE);
                    return false;
                }
        
                HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
                if (!kernel32) {
                    VirtualFreeEx(process::handle, mem, 0, MEM_RELEASE);
                    return false;
                }
        
                LPVOID lla = GetProcAddress(kernel32, "LoadLibraryA");
                if (!lla) {
                    VirtualFreeEx(process::handle, mem, 0, MEM_RELEASE);
                    return false;
                }
        
                HANDLE thread = CreateRemoteThread( process::handle, nullptr, 0, (LPTHREAD_START_ROUTINE)lla, mem, 0, nullptr);
        
                if (!thread) {
                    VirtualFreeEx(process::handle, mem, 0, MEM_RELEASE);
                    return false;
                }
                
                WaitForSingleObject(thread, 2000);
                
                CloseHandle(thread);
                VirtualFreeEx(process::handle, mem, 0, MEM_RELEASE);
        
               return true;
        }
    }
}