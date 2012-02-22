#ifndef HANDLER_H_
#define HANDLER_H_

#include <QObject>

#include "Packet.h"

class Handler: public QObject {
	Q_OBJECT

public:
	Handler(QObject* = 0);

	virtual bool receiveFromClient(const QByteArray&) = 0;
	virtual bool receiveFromServer(const QByteArray&) = 0;
};

#endif /* HANDLER_H_ */
