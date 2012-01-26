#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "ServerHandler.h"
#include "Hook.h"

ServerHandler::ServerHandler(Hook* hook) :
		_hook(hook), _server(new QTcpServer(_hook)), _currentSocket(NULL) {
	connect(_server, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
}

bool ServerHandler::listen(const QHostAddress& address, quint16 port) {
	return _server->listen(address, port);
}

bool ServerHandler::event(QEvent* event) {
	if (event->type() == OutgoingMessageEventType) {
		OutgoingMessageEvent* outputMessageEvent = (OutgoingMessageEvent*) event;
		handleOutgoingMessage(outputMessageEvent->message());
		return true;
	}
	return QObject::event(event);
}

void ServerHandler::acceptNewConnection() {
	qDebug() << Q_FUNC_INFO;

	QTcpSocket* connection = _server->nextPendingConnection();
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

void ServerHandler::handleOutgoingMessage(const EncryptedMessage& message) {
	if (!handleOutgoingMessageInternal(message)) {
		qDebug() << "message:" << message.length();
		_hook->write(message);
	}
}

bool ServerHandler::handleOutgoingMessageInternal(const EncryptedMessage& message) {
	return false;
}

void ServerHandler::handleIncomingMessage(const EncryptedMessage& message) {
}

bool ServerHandler::handleIncomingMessageInternal(const EncryptedMessage& message) {
	return false;
}
