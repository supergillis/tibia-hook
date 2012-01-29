#ifndef HANDLER_H_
#define HANDLER_H_

#include <QObject>
#include <QEvent>

#include "EncryptedMessage.h"

class Handler: public QObject {
	Q_OBJECT

public:
	Handler(QObject* = 0);

	bool event(QEvent*);

	virtual void receiveFromClient(const EncryptedMessage*) = 0;
	virtual void receiveFromServer(const EncryptedMessage*) = 0;
};

#endif /* HANDLER_H_ */
