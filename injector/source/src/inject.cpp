#include "inject.h"
#include "process.h"
#include <windows.h>
#include <iostream>

namespace injector {
    namespace inject {
        bool inject(const std::string& dll) {

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
        
                WaitForSingleObject(thread, INFINITE);
                
                CloseHandle(thread);
                VirtualFreeEx(process::handle, mem, 0, MEM_RELEASE);
        
               return true;
        }
    }
}