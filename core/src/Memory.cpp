#include "Memory.h"

#ifdef Q_OS_WIN

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Tlhelp32.h>

DWORD calculateModuleBase() {
    MODULEENTRY32 moduleEntry = {0};
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    if (!snapShot) {
        return 0;
    }

    moduleEntry.dwSize = sizeof(moduleEntry);
    BOOL currentModule = Module32First(snapShot, &moduleEntry);
    DWORD base = 0;

    if (currentModule) {
        static char hold[512];
        memcpy(hold, moduleEntry.szModule, strlen(moduleEntry.szModule) + 1);

        if(std::string(hold).find(".exe") == std::string(hold).size() - 4) {
            base = (DWORD) moduleEntry.modBaseAddr;
        }
    }

    CloseHandle(snapShot);

    return base;
}

MemoryLocation Memory::mapAddress(MemoryLocation address) const {
    return staticMapAddress(address);
}

MemoryLocation Memory::staticMapAddress(MemoryLocation address) {
    static DWORD base = (DWORD) calculateModuleBase();
    static DWORD windowsXPBase = 0x400000;
    static MemoryLocation offset = (MemoryLocation) (base - windowsXPBase);
    return address + offset;
}

#else

MemoryLocation Memory::mapAddress(MemoryLocation address) const {
    return address;
}

MemoryLocation Memory::staticMapAddress(MemoryLocation address) {
    return address;
}

#endif
