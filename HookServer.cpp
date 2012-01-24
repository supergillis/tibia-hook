#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "HookServer.h"
#include "Hook.h"

HookServer::HookServer(Hook* hook) :
		QTcpServer(hook), _hook(hook), _currentSocket(NULL) {
	connect(this, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
}

bool HookServer::event(QEvent* event) {
	if (event->type() == OutgoingMessageEventType) {
		OutgoingMessageEvent* outputMessageEvent = (OutgoingMessageEvent*) event;
		handleOutgoingMessage(outputMessageEvent->message());
		return true;
	}
	return QTcpServer::event(event);
}

void HookServer::acceptNewConnection() {
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

void HookServer::socketClosed() {
	qDebug() << Q_FUNC_INFO;

	_currentSocket->deleteLater();
	_currentSocket = NULL;
}

void HookServer::handleOutgoingMessage(const Message& message) {
	if (!handleOutgoingMessageInternal(message)) {
		_hook->write(message);
	}
}

bool HookServer::handleOutgoingMessageInternal(const Message& message) {
	return false;
}

void HookServer::handleIncomingMessage(const Message& message) {
}

bool HookServer::handleIncomingMessageInternal(const Message& message) {
	return false;
}
