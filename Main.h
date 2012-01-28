#ifndef MAIN_H_
#define MAIN_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <X11/Xlib.h>

int __connect(int, const struct sockaddr*, socklen_t);

ssize_t __read(int, void*, size_t);

ssize_t __write(int, const void*, size_t);

Window __XCreateWindow(Display*, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*);

int __XNextEvent(Display*, XEvent*);

#endif /* MAIN_H_ */
