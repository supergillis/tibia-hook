#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <queue>
#include <pthread.h>

#include "thread.h"
#include "message.h"

class TcpServerProvider: public Thread {
public:
	TcpServerProvider(uint16_t);

	void run();
	void send(const Message& message);

private:
	void connection(void*);
	void server(void*);

	bool _running;
	uint16_t _port;
	uint8_t _max_connections;

	pthread_mutex_t _messages_lock;
	std::queue<Message> messages;
};

#endif /* PROVIDER_H_ */
