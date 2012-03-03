#ifndef SENDERINTERFACE_H_
#define SENDERINTERFACE_H_

#include <QByteArray>

class SenderInterface {
public:
	virtual ~SenderInterface() {}

	virtual void sendToClient(const QByteArray&) = 0;
	virtual void sendToServer(const QByteArray&) = 0;
};

#endif
