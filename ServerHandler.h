#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <QEvent>
#include <QTcpServer>
#include <QTcpSocket>

#include "Handler.h"
#include "EncryptedMessage.h"
#include "DecryptedMessage.h"

class Hook;
class ServerHandler: public Handler {
	Q_OBJECT

public:
	ServerHandler(Hook*);

	bool listen(const QHostAddress& = QHostAddress::Any, quint16 = 0);

	void handleOutgoingMessage(const EncryptedMessage&);
	bool handleOutgoingMessageInternal(const EncryptedMessage&);

	void handleIncomingMessage(const EncryptedMessage&);
	bool handleIncomingMessageInternal(const EncryptedMessage&);

private slots:
	void acceptNewConnection();
	void socketClosed();

private:
	QTcpServer* _server;
	QTcpSocket* _currentSocket;
};

#endif /* PROVIDER_H_ */
