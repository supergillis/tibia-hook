#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <QEvent>
#include <QTcpServer>
#include <QTcpSocket>

#include "Message.h"
#include "Packet.h"

class Hook;
class HookServer: public QTcpServer {
	Q_OBJECT

public:
	HookServer(Hook*);

	bool event(QEvent*);

	void handleOutgoingMessage(const Message&);
	bool handleOutgoingMessageInternal(const Message&);

	void handleIncomingMessage(const Message&);
	bool handleIncomingMessageInternal(const Message&);

private slots:
	void acceptNewConnection();
	void socketClosed();

private:
	Hook* _hook;
	QTcpSocket* _currentSocket;
};

#endif /* PROVIDER_H_ */
