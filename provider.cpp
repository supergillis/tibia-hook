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
	pthread_mutex_init(&_messages_lock, NULL);
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
			while (!requested_stop()) {
				pthread_mutex_lock(&_messages_lock);
				while (!messages.empty()) {
					Message message = messages.front();
					printf("parsing message with length %d\n", message.getBufferLength());
					messages.pop();
				}
				pthread_mutex_unlock(&_messages_lock);
			}
		} else {
			printf("error on accepting\n");
		}
		close(accept_socket);
	}

	close(listen_socket);
}

void TcpServerProvider::send(const Message& message) {
	messages.push(message);
}
