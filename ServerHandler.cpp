#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "ServerHandler.h"
#include "Hook.h"

ServerHandler::ServerHandler(Hook* hook) :
		QTcpServer(hook), _hook(hook), _currentSocket(NULL) {
	connect(this, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
}

bool ServerHandler::event(QEvent* event) {
	if (event->type() == OutgoingMessageEventType) {
		OutgoingMessageEvent* outputMessageEvent = (OutgoingMessageEvent*) event;
		handleOutgoingMessage(outputMessageEvent->message());
		return true;
	}
	return QTcpServer::event(event);
}

void ServerHandler::acceptNewConnection() {
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

void ServerHandler::socketClosed() {
	qDebug() << Q_FUNC_INFO;

	_currentSocket->deleteLater();
	_currentSocket = NULL;
}

void ServerHandler::handleOutgoingMessage(const Message& message) {
	if (!handleOutgoingMessageInternal(message)) {
		qDebug() << "message:" << message.length();
		_hook->write(message);
	}
}

bool ServerHandler::handleOutgoingMessageInternal(const Message& message) {
	return false;
}

void ServerHandler::handleIncomingMessage(const Message& message) {
}

bool ServerHandler::handleIncomingMessageInternal(const Message& message) {
	return false;
}
