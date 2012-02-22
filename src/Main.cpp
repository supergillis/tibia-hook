#include <pthread.h>

#include "Main.h"
#include "Hook.h"
#include "ScriptHandler.h"

void hook_constructor() __attribute__((constructor));
void hook_destructor() __attribute__((destructor));
void* hook_thread(void*);

pthread_t hook_id;

void hook_constructor() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&hook_id, &attr, hook_thread, NULL);
}

void hook_destructor() {
	pthread_join(hook_id, NULL);
}

#include "DebuggerModule.h"
#include "ClassModule.h"
#include "EnvironmentModule.h"
#include "MemoryModule.h"
#include "NetworkModule.h"
#include "PacketModule.h"
#include "SchedulerModule.h"

void* hook_thread(void*) {
	Hook* hook = new Hook();
	ScriptHandler* handler = new ScriptHandler(hook);
	handler->install(new DebuggerModule(hook));
	handler->install(new ClassModule(hook));
	handler->install(new EnvironmentModule(hook));
	handler->install(new PacketModule(hook));
	handler->install(new MemoryModule(hook));
	handler->install(new NetworkModule(hook));
	handler->install(new SchedulerModule(hook));
	handler->reload();
	hook->setHandler(handler);
	hook->exec();
	return NULL;
}
