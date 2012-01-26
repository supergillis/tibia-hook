#ifndef MAIN_HANDLER_H
#define MAIN_HANDLER_H

#include <unistd.h>
#include <string.h>

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>

#include "EncryptedMessage.h"

class Handler;
class Hook: public QCoreApplication {
	Q_OBJECT

public:
	Hook();
	~Hook();

	const int socket() const;
	void setSocket(const int);

	ssize_t read(quint8*, ssize_t);

	ssize_t write(const quint8*, ssize_t);
	ssize_t write(const EncryptedMessage&);

	ssize_t hookOutgoingMessage(const quint8*, ssize_t);
	ssize_t hookIncomingMessage(quint8*, ssize_t);

private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	int _socket;

	Handler* _handler;
	bool _loggedIn;
	bool _pendingLogin;
	quint8 _protocol;
};

static const QEvent::Type OutgoingMessageEventType = QEvent::User;

class OutgoingMessageEvent: public QEvent {
public:
	OutgoingMessageEvent(const EncryptedMessage& message): QEvent(OutgoingMessageEventType), _message(message) {
		// Do nothing
	}

	const EncryptedMessage& message() {
		return _message;
	}

private:
	const EncryptedMessage _message;
};

#endif
