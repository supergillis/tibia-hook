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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Application.h"

HANDLE thread_handle;
DWORD thread_id;

Application* application = NULL;

/**
  * This function runs when the thread is created. Qt runs in this thread.
  */
DWORD WINAPI hook_thread(LPVOID) {
	// Create the application and enter the main event loop
    application = new Application();
    application->initialize();
	application->exec();

	// When the application is done executing, clean up
	delete application;
	application = NULL;

    return NULL;
}

LRESULT CALLBACK hook_callback(int nCode, WPARAM wParam, LPARAM lParam) {
    static bool injected = false;
    if (!injected) {
        injected = true;
        thread_handle = CreateThread(NULL, 0, hook_thread, NULL, 0, &thread_id);
    }
	return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C"
{
    /**
      * This function runs when the library is injected.
      */
    __declspec(dllexport) void hook_constructor(HINSTANCE hMod, DWORD dwThreadId) {
        ::SetWindowsHookEx(WH_GETMESSAGE, hook_callback, hMod, dwThreadId);
        ::PostThreadMessage(dwThreadId, WM_NULL, 0, 0);
    }
}
