#ifndef RECEIVERINTERFACE_H
#define RECEIVERINTERFACE_H

#include <QByteArray>

class ReceiverInterface {
public:
	virtual ~ReceiverInterface() {}

	virtual bool receiveFromClient(const QByteArray&) = 0;
	virtual bool receiveFromServer(const QByteArray&) = 0;
};

#endif
