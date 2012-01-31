#include "Hook.h"
#include "HookSocket.h"
#include "HookClient.h"
#include "LinuxHookSocket.h"
#include "LinuxHookClient.h"
#include "ScriptHandler.h"
#include "Main.h"

#include "DebuggerModule.h"
#include "ClassModule.h"
#include "ClientModule.h"
#include "EnvironmentModule.h"
#include "MemoryModule.h"
#include "NetworkModule.h"
#include "PacketModule.h"

#include <assert.h>
#include <dlfcn.h>
#include <pthread.h>
#include <X11/Xlib.h>

Window __XCreateWindow(Display*, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*);

void hook_constructor() __attribute__((constructor));
void hook_destructor() __attribute__((destructor));

Hook* hook = NULL;
LinuxHookSocket* hookSocket = NULL;
LinuxHookClient* hookClient = NULL;

pthread_t hook_id;

void* hook_thread(void*) {
	hook = new Hook();
	ScriptHandler* handler = new ScriptHandler(hook);
	handler->install(new DebuggerModule(hook));
	handler->install(new ClassModule(hook));
	handler->install(new EnvironmentModule(hook));
	handler->install(new PacketModule(hook));
	handler->install(new ClientModule(hook));
	handler->install(new MemoryModule(hook));
	handler->install(new NetworkModule(hook));
	handler->reload();
	hook->setHandler(handler);
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

int connect(int fd, const struct sockaddr* address, socklen_t length) {
	static int X11Socket = -1;
	if (X11Socket == -1) {
		X11Socket = fd;
	}
	else if (hook) {
		if (hookSocket != NULL) {
			delete hookSocket;
		}
		hookSocket = new LinuxHookSocket(hook, fd);
		hook->setSocket(hookSocket);
	}
	return __connect(fd, address, length);
}

int poll(struct pollfd* fds, nfds_t nfds, int timeout) {
	if (nfds == 1 && fds[0].events == POLLIN && hookSocket && hookSocket->fileDescriptor() == fds[0].fd && hook->hasClientMessages()) {
		fds[0].revents = POLLIN;
		return 1;
	}
	return __poll(fds, nfds, timeout);
}

ssize_t read(int fd, void* buffer, size_t length) {
	if (hookSocket && hookSocket->fileDescriptor() == fd && length > 0) {
		return hook->receiveFromServer((quint8*) buffer, length);
	}
	return __read(fd, buffer, length);
}

ssize_t write(int fd, const void* buffer, size_t length) {
	if (hookSocket && hookSocket->fileDescriptor() == fd && length > 0) {
		return hook->receiveFromClient((const quint8*) buffer, length);
	}
	return __write(fd, buffer, length);
}

Window XCreateWindow(Display* display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class_, Visual* visual, unsigned long valuemask, XSetWindowAttributes* attributes) {
	Window window = __XCreateWindow(display, parent, x, y, width, height, border_width, depth, class_, visual, valuemask, attributes);
	if (hookClient == NULL) {
		hookClient = new LinuxHookClient(hook, display, window);
		hook->setClient(hookClient);
	}
	return window;
}

int __connect(int socket, const struct sockaddr* address, socklen_t length) {
	static int (*original)(int, const struct sockaddr*, socklen_t);
	if (original == NULL) {
		original = (int(*)(int, const struct sockaddr*, socklen_t)) dlsym(RTLD_NEXT, "connect");assert(original != NULL);
	}
	return original(socket, address, length);
}

int __poll(struct pollfd* fds, nfds_t nfds, int timeout) {
	static int (*original)(struct pollfd*, nfds_t, int);
	if (original == NULL) {
		original = (int(*)(struct pollfd*, nfds_t, int)) dlsym(RTLD_NEXT, "poll");assert(original != NULL);
	}
	return original(fds, nfds, timeout);
}

ssize_t __read(int socket, void* buffer, size_t length) {
	static ssize_t (*original)(int, void*, size_t);
	if (original == NULL) {
		original = (ssize_t(*)(int, void*, size_t)) dlsym(RTLD_NEXT, "read");assert(original != NULL);
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

Window __XCreateWindow(Display* display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class_, Visual* visual, unsigned long valuemask, XSetWindowAttributes* attributes) {
	static Window (*original)(Display*, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*);
	if (original == NULL) {
		original = (Window(*)(Display*, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*)) dlsym(RTLD_NEXT, "XCreateWindow");assert(original != NULL);
	}
	return original(display, parent, x, y, width, height, border_width, depth, class_, visual, valuemask, attributes);
}
