/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include <windows.h>
#include <Tlhelp32.h>

#include "Memory.h"

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

MemoryLocation Memory::rebase(MemoryLocation address) const {
    return Memory::staticRebase(address);
}

MemoryLocation Memory::staticRebase(MemoryLocation address) {
    static DWORD moduleBase = calculateModuleBase();
    return (address - 0x400000) + moduleBase;
}
