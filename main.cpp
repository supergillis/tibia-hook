#define _GNU_SOURCE

#include <dlfcn.h>
#include <assert.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "main.h"
#include "hook.h"
#include "thread.h"
#include "provider.h"

void hook_constructor() __attribute__((constructor));
void hook_destructor() __attribute__((destructor));

TcpServerProvider* provider;
Hook* hook;

int x_socket = -1;
int game_socket = -1;

void hook_constructor() {
	provider = new TcpServerProvider(7170);
	provider->start();
	hook = new Hook(provider);
}

void hook_destructor() {
	provider->request_stop();
}

int connect(int socket, const struct sockaddr* address, socklen_t length) {
	if (x_socket == -1)
		x_socket = socket;
	else
		game_socket = socket;
	return __connect(socket, address, length);
}

ssize_t read(int socket, void* buffer, size_t length) {
	if (game_socket == socket && length > 0)
		return hook->hookIncomingPacket(socket, (uint8_t*) buffer, length);
	return __read(socket, buffer, length);
}

ssize_t write(int socket, const void* buffer, size_t length) {
	if (game_socket == socket && length > 0)
		return hook->hookOutgoingPacket(socket, (const uint8_t*) buffer, length);
	return __write(socket, buffer, length);
}

int __connect(int socket, const struct sockaddr* address, socklen_t length) {
	static int (*original)(int, const struct sockaddr*, socklen_t);
	if (original == NULL) {
		original = (int(*)(int, const struct sockaddr*, socklen_t)) dlsym(RTLD_NEXT, "connect");assert(original != NULL);
	}
	return original(socket, address, length);
}

ssize_t __read(int socket, void* buffer, size_t length) {
	static ssize_t (*original)(int, const void*, size_t);
	if (original == NULL) {
		original = (ssize_t(*)(int, const void*, size_t)) dlsym(RTLD_NEXT, "read");assert(original != NULL);
	}
	return original(socket, buffer, length);
}

ssize_t __write(int socket, const void* buffer, size_t length) {
	static ssize_t (*original)(int, const void*, size_t);
	if (original == NULL) {
		original = (ssize_t(*)(int, const void*, size_t)) dlsym(RTLD_NEXT, "write");assert(original != NULL);
	}
	return original(socket, buffer, length);
}
