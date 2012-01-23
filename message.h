#ifndef INPUTMESSAGE_H
#define INPUTMESSAGE_H

#include <stdint.h>
#include <string>

#include "position.h"

class Message
{
public:
	Message(const uint8_t*, uint16_t);
	Message(const Message&);
	~Message();

	bool decrypt(uint32_t[]);
	bool paste(const uint8_t*, uint16_t);

	bool canRead(uint16_t = 1);
	bool needsMoreData() {return _needs_more > 0;}

	uint16_t getBufferLength();
	uint16_t getBufferPosition();
	void setBufferPosition(uint16_t);
	void skipBytes(uint16_t);

	uint8_t getByte(bool = false);
	uint16_t getU16(bool = false);
	uint32_t getU32(bool = false);
	uint64_t getU64(bool = false);
	std::string getString();
	Position getPosition();

protected:
	uint8_t* _buffer;
	uint32_t _checksum;
	uint16_t _length;
	uint16_t _position;
	uint16_t _needs_more;
};

#endif // INPUTMESSAGE_H
