#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "main.h"
#include "provider.h"
#include "thread.h"

TcpServerProvider::TcpServerProvider(uint16_t port) :
		Thread(), _running(false), _port(port), _max_connections(10) {
}

void TcpServerProvider::run() {
	struct sockaddr_in serv_addr;
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket < 0) {
		printf("error opening socket\n");
		return;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(_port);

	if (bind(listen_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("error on binding\n");
		return;
	}

	if (listen(listen_socket, _max_connections) < 0) {
		printf("error on listening\n");
		return;
	}

	while (!requested_stop()) {
		int accept_socket = accept(listen_socket, NULL, NULL);
		if (accept_socket > 0) {
			TcpServerConnection* connection = new TcpServerConnection(accept_socket);
			connection->start();
		} else {
			printf("error on accepting\n");
		}
	}

	close(listen_socket);
}

TcpServerConnection::TcpServerConnection(int socket) :
		Thread(), _socket(socket) {
}

void TcpServerConnection::run() {
	printf("connection on socket %d\n", _socket);

	char buffer[256];
	bzero(buffer, 256);
	int length = __read(_socket, buffer, 255);
	printf("received: %d bytes\n%s\n", length, buffer);

	close(_socket);
}
