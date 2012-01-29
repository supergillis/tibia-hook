#ifndef MAIN_H_
#define MAIN_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>

int __connect(int, const struct sockaddr*, socklen_t);

int __poll(struct pollfd*, nfds_t, int);

ssize_t __read(int, void*, size_t);

ssize_t __write(int, const void*, size_t);

#endif /* MAIN_H_ */
