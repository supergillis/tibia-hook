#include "LinuxHookSocket.h"
#include "Main.h"

LinuxHookSocket::LinuxHookSocket(Hook* hook, int fileDescriptor) :
		HookSocket(hook), fileDescriptor_(fileDescriptor) {
}

int LinuxHookSocket::fileDescriptor() const {
	return fileDescriptor_;
}

ssize_t LinuxHookSocket::read(quint8* buffer, ssize_t length) {
	return __read(fileDescriptor_, buffer, length);
}

ssize_t LinuxHookSocket::write(const quint8* buffer, ssize_t length) {
	return __write(fileDescriptor_, buffer, length);
}
