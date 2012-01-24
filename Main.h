#ifndef HOOK_H_
#define HOOK_H_

#include <sys/socket.h>
#include <sys/types.h>

int __connect(int socket, const struct sockaddr* address, socklen_t length);

ssize_t __read(int socket, void* buffer, size_t length);

ssize_t __write(int socket, const void* buffer, size_t length);

#endif /* HOOK_H_ */
