#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <QEvent>
#include <QTcpServer>
#include <QTcpSocket>

#include "message.h"

class Hook;
class Provider: public QTcpServer {
	Q_OBJECT

public:
	Provider(Hook*);

	bool event(QEvent* event);

	void handleOutgoingMessage(const Message& message);
	bool handleOutgoingMessageInternal(const Message& message);

	void handleIncomingMessage(const Message& message);
	bool handleIncomingMessageInternal(const Message& message);

private slots:
	void acceptNewConnection();
	void socketClosed();

private:
	Hook* _hook;
	QTcpSocket* _currentSocket;
};

#endif /* PROVIDER_H_ */
