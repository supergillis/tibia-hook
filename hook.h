#ifndef MAIN_HANDLER_H
#define MAIN_HANDLER_H

#define BATTLELIST_START 0x855e6c0
#define BATTLELIST_LENGTH 160
#define BATTLELIST_COUNT 250

/* 9.44 addresses */
#define XTEA_START 0x85d8830
#define XTEA_LENGTH 0x04

/* 8.62 addresses
 #define XTEA_START 0x859bcd0
 #define XTEA_LENGTH 0x04*/

/* 8.52 addresses
 #define XTEA_START 0x856f450
 #define XTEA_LENGTH 0x04 */

/* 8.53 addresses
 #define XTEA_START 0x85715f0
 #define XTEA_LENGTH 0x04 */

#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include <QCoreApplication>
#include <QEvent>

#include "message.h"
#include "provider.h"

class Hook: public QCoreApplication {
	Q_OBJECT

public:
	Hook();
	~Hook();

	const int socket() const;
	void setSocket(const int);

	ssize_t read(uint8_t*, ssize_t);
	ssize_t write(const uint8_t*, ssize_t);

	ssize_t write(const Message& message);

	ssize_t hookOutgoingPacket(const uint8_t*, ssize_t);
	ssize_t hookIncomingPacket(uint8_t*, ssize_t);

private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	int _socket;

	Provider* _provider;
	bool _loggedIn;
	bool _pendingLogin;
	uint32_t* _key;
	uint8_t _protocol;
};

static const QEvent::Type OutgoingMessageEventType = QEvent::User;

class OutgoingMessageEvent: public QEvent {
public:
	OutgoingMessageEvent(const Message&);

	const Message& message() {
		return _message;
	}

private:
	const Message _message;
};

#endif
