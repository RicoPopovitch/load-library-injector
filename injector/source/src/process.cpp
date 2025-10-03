#include "process.h"
#include <tlhelp32.h>

namespace injector {
    namespace process {
        HANDLE handle;
        DWORD procid;

        bool get_process(const std::string& process) {
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap == INVALID_HANDLE_VALUE) {
                return false;
            }

            PROCESSENTRY32 pe;
            pe.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(snap, &pe)) {
                do {
                    if (process == pe.szExeFile) {
                        procid = pe.th32ProcessID;
                        CloseHandle(snap);
                        return true;
                    }
                } while (Process32Next(snap, &pe));
            }

            CloseHandle(snap);
            return false;
        }
    
        bool open_process() {
            handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procid);
            if (!handle) {
                return false;
            }
            return true;
        }
    }
}