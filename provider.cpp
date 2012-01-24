#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "main.h"
#include "hook.h"
#include "provider.h"

Provider::Provider(Hook* hook) :
		QTcpServer(hook), _hook(hook), _currentSocket(NULL) {
	connect(this, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
}

bool Provider::event(QEvent* event) {
	if (event->type() == OutgoingMessageEventType) {
		OutgoingMessageEvent* outputMessageEvent = (OutgoingMessageEvent*) event;
		handleOutgoingMessage(outputMessageEvent->message());
		return true;
	}
	return QTcpServer::event(event);
}

void Provider::acceptNewConnection() {
	qDebug() << Q_FUNC_INFO;
	QTcpSocket* connection = nextPendingConnection();
	if (_currentSocket == NULL) {
		_currentSocket = connection;
		connect(_currentSocket, SIGNAL(disconnected()), this, SLOT(socketClosed()));
	}
	else {
		connection->close();
	}
}

void Provider::socketClosed() {
	qDebug() << Q_FUNC_INFO;
	_currentSocket->deleteLater();
	_currentSocket = NULL;
}

void Provider::handleOutgoingMessage(const Message& message) {
	//if(!handleOutgoingMessageInternal(socket, message))
	qDebug() << "handleOutgoingMessage" << message.length();
	_hook->write(message);
}

bool Provider::handleOutgoingMessageInternal(const Message& message) {
	if (_currentSocket == NULL) {
		qDebug() << "no connection";
		return false;
	}

	_currentSocket->write((const char*) message.data());
	_currentSocket->flush();

	if (!_currentSocket->waitForReadyRead(50)) {
		qDebug() << "no answer";
		return false;
	}

	QByteArray answer = _currentSocket->readAll();

	// Do something

	return false;
}

void Provider::handleIncomingMessage(const Message& message) {
}

bool Provider::handleIncomingMessageInternal(const Message& message) {
	return false;
}
