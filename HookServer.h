#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <QEvent>
#include <QTcpServer>
#include <QTcpSocket>

#include "EncryptedMessage.h"
#include "DecryptedMessage.h"

class Hook;
class HookServer: public QTcpServer {
	Q_OBJECT

public:
	HookServer(Hook*);

	bool event(QEvent*);

	void handleOutgoingMessage(const EncryptedMessage&);
	bool handleOutgoingMessageInternal(const EncryptedMessage&);

	void handleIncomingMessage(const EncryptedMessage&);
	bool handleIncomingMessageInternal(const EncryptedMessage&);

private slots:
	void acceptNewConnection();
	void socketClosed();

private:
	Hook* _hook;
	QTcpSocket* _currentSocket;
};

#endif /* PROVIDER_H_ */
