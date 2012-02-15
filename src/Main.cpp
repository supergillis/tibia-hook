#include "Hook.h"
#include "ScriptHandler.h"
#include "Main.h"

#include "DebuggerModule.h"
#include "ClassModule.h"
#include "EnvironmentModule.h"
#include "MemoryModule.h"
#include "NetworkModule.h"
#include "PacketModule.h"
#include "SchedulerModule.h"

#include <assert.h>
#include <pthread.h>

void hook_constructor() __attribute__((constructor));
void hook_destructor() __attribute__((destructor));
void* hook_thread(void*);

typedef void (*writePacketSignature)(bool);
typedef void (*readPacketSignature)();

MologieDetours::Detour<writePacketSignature>* writePacketDetour;
MologieDetours::Detour<readPacketSignature>* readPacketDetour;

void writePacketHook(bool);
void readPacketHook();

Hook* hook;
pthread_t hook_id;

void hook_constructor() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&hook_id, &attr, hook_thread, NULL);

	writePacketDetour = new MologieDetours::Detour<writePacketSignature>((writePacketSignature) ADDRESS_WRITE_PACKET, writePacketHook);
	//readPacketDetour = new MologieDetours::Detour<readPacketSignature>((readPacketSignature) ADDRESS_READ_PACKET, readPacketHook);
}

void hook_destructor() {
	pthread_join(hook_id, NULL);
}

void* hook_thread(void*) {
	hook = new Hook();
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

void writePacketHook(bool encrypt) {
	if (hook != NULL) {
		hook->receiveFromClient(encrypt);
	}
}

void readPacketHook() {
	if (hook != NULL) {
		hook->receiveFromServer();
	}
}

void writePacket(bool encrypt) {
	writePacketDetour->GetOriginalFunction()(encrypt);
}

void readPacket() {
	readPacketDetour->GetOriginalFunction()();
}
