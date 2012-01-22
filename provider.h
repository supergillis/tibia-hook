#ifndef PROVIDER_H_
#define PROVIDER_H_

#include "thread.h"

class TcpServerProvider: public Thread {
public:
	TcpServerProvider(uint16_t);

	void run();

private:
	void connection(void*);
	void server(void*);

	bool _running;
	uint16_t _port;
	uint8_t _max_connections;
};

class TcpServerConnection: public Thread {
public:
	TcpServerConnection(int);

	void run();

private:
	int _socket;
};

#endif /* PROVIDER_H_ */
