#ifndef HOOKSOCKET_H_
#define HOOKSOCKET_H_

#include <QtGlobal>

#include <sys/types.h>

class Hook;
class HookSocket {
public:
	HookSocket(Hook*);

	virtual ssize_t read(quint8*, ssize_t) = 0;
	virtual ssize_t write(const quint8*, ssize_t) = 0;

private:
	Hook* hook_;
};

#endif /* HOOKSOCKET_H_ */
