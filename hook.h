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

#include "packet_handler.h"
#include "message.h"
#include "provider.h"

class Hook
{
public:
	Hook(TcpServerProvider*);
	~Hook();

	void setLoggedIn(bool value) {_loggedIn = value;}

	ssize_t hookOutgoingPacket(int, const uint8_t*, ssize_t);
	ssize_t hookIncomingPacket(int, uint8_t*, ssize_t);
protected:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	bool _initialized;
	bool _pendingLogin;
	bool _loggedIn;

	uint8_t _protocol;
	uint32_t* _key;

	TcpServerProvider* _provider;
	IncomingPacketHandler _incoming;
	OutgoingPacketHandler _outgoing;
};

#endif
