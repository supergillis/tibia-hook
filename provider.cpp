#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "provider.h"

Provider::Provider() :
		QTcpServer() {
	connect(this, SIGNAL(newConnection()), this, SLOT(pendingConnection()));
	qDebug() << "listening";
}

void Provider::pendingConnection() {
	QTcpSocket* connection = nextPendingConnection();
	qDebug() << "new connection";
}
