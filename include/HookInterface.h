#ifndef HOOKINTERFACE_H_
#define HOOKINTERFACE_H_

#include <QByteArray>

class HookInterface {
public:
	virtual ~HookInterface() {}

	virtual void sendToClient(const QByteArray&) = 0;
	virtual void sendToServer(const QByteArray&) = 0;
};

#endif /* HOOKINTERFACE_H_ */
