#ifndef LINUXHOOKSOCKET_H_
#define LINUXHOOKSOCKET_H_

#include "HookSocket.h"

class LinuxHookSocket: public HookSocket {
public:
	LinuxHookSocket(Hook*, int);

	int fileDescriptor() const;

	ssize_t read(quint8*, ssize_t);
	ssize_t write(const quint8*, ssize_t);

private:
	int fileDescriptor_;
};

#endif /* LINUXHOOKSOCKET_H_ */
