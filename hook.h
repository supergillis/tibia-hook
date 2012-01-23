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

#include "message.h"
#include "provider.h"

class Hook: public QCoreApplication
{
public:
	Hook();
	~Hook();

	void setLoggedIn(bool value) {_loggedIn = value;}

	ssize_t hookOutgoingPacket(int, const uint8_t*, ssize_t);
	ssize_t hookIncomingPacket(int, uint8_t*, ssize_t);
protected:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	Provider* _provider;

	uint8_t _protocol;
	uint32_t* _key;

	bool _pendingLogin;
	bool _loggedIn;

};

#endif
