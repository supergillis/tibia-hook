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

void* hook_thread(void*) {
	Hook* hook = new Hook();
	ScriptHandler* handler = new ScriptHandler(hook);
	hook->setHandler(handler);
	hook->exec();
	return NULL;
}
