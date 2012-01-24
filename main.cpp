#include <assert.h>
#include <dlfcn.h>
#include <pthread.h>

#include "main.h"
#include "hook.h"

void hook_constructor() __attribute__((constructor));
void hook_destructor() __attribute__((destructor));

Hook* hook;
pthread_t hook_id;

void* hook_thread(void*) {
	hook = new Hook();
	hook->exec();
	return NULL;
}

void hook_constructor() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&hook_id, &attr, hook_thread, NULL);
}

void hook_destructor() {
	pthread_join(hook_id, NULL);
}

int x_socket = -1;
int game_socket = -1;

int connect(int socket, const struct sockaddr* address, socklen_t length) {
	if (x_socket == -1) {
		x_socket = socket;
	}
	else {
		game_socket = socket;
		hook->setSocket(game_socket);
	}
	return __connect(socket, address, length);
}

ssize_t read(int socket, void* buffer, size_t length) {
	if (hook && hook->socket() == socket && length > 0)
		return hook->hookIncomingPacket((uint8_t*) buffer, length);
	return __read(socket, buffer, length);
}

ssize_t write(int socket, const void* buffer, size_t length) {
	if (hook && hook->socket() == socket && length > 0)
		return hook->hookOutgoingPacket((const uint8_t*) buffer, length);
	return __write(socket, buffer, length);
}

int __connect(int socket, const struct sockaddr* address, socklen_t length) {
	static int (*original)(int, const struct sockaddr*, socklen_t);
	if (original == NULL) {
		original = (int(*)(int, const struct sockaddr*, socklen_t)) dlsym(RTLD_NEXT, "connect");
		assert(original != NULL);
	}
	return original(socket, address, length);
}

ssize_t __read(int socket, void* buffer, size_t length) {
	static ssize_t (*original)(int, const void*, size_t);
	if (original == NULL) {
		original = (ssize_t(*)(int, const void*, size_t)) dlsym(RTLD_NEXT, "read");
		assert(original != NULL);
	}
	return original(socket, buffer, length);
}

ssize_t __write(int socket, const void* buffer, size_t length) {
	static ssize_t (*original)(int, const void*, size_t);
	if (original == NULL) {
		original = (ssize_t(*)(int, const void*, size_t)) dlsym(RTLD_NEXT, "write");
		assert(original != NULL);
	}
	return original(socket, buffer, length);
}
