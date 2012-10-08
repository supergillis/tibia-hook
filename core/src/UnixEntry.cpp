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

#include <pthread.h>
#include <vector> // Necessary to fix Qt errors on Windows

#include "Application.h"
#include "Hook.h"

pthread_t hook_id;
Application* application = NULL;

void hook_destructor() __attribute__((destructor));
void hook_constructor() __attribute__((constructor));
void* hook_thread(void*);

/**
  * This function runs when the program exits.
  */
void hook_destructor() {
    // This doesn't work as expected yet
    return;

    // Exit the application
    application->exit();

    // Wait for the application to exit
    pthread_join(hook_id, NULL);
}

/**
  * This function runs when the library is injected.
  */
void hook_constructor() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&hook_id, &attr, hook_thread, NULL);
}

/**
  * This function runs when the thread is created. Qt runs in this thread.
  */
void* hook_thread(void*) {
    // Create the application and enter the main event loop
    application = new Application();
    application->exec();

    // When the application is done executing, clean up
    delete application;
    application = NULL;

    return NULL;
}
