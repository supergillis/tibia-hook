#ifndef HANDLER_H_
#define HANDLER_H_

#include <QObject>

#include "EncryptedMessage.h"

class Handler: public QObject {
	Q_OBJECT

public:
	virtual void handleOutgoingMessage(const EncryptedMessage&) = 0;
	virtual void handleIncomingMessage(const EncryptedMessage&) = 0;
};

#endif /* HANDLER_H_ */
