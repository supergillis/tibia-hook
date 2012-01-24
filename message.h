#ifndef INPUTMESSAGE_H
#define INPUTMESSAGE_H

#include <stdint.h>

#include <QByteArray>

class Message
{
public:
	Message(const uint8_t*, uint16_t);

	uint8_t* data();
	const uint8_t* constData() const;

	bool decrypt(uint32_t[]);
	bool encrypt(uint32_t[]);

	uint16_t length() const;
	uint16_t position() const;
	void setPosition(uint16_t);
	void skip(int32_t);

	bool canRead(uint16_t = 1) const;

	uint8_t readByte() throw();
	uint16_t readU16();
	uint32_t readU32();

private:
	QByteArray _buffer;
	uint32_t _checksum;
	uint16_t _length;
	uint16_t _position;
	uint16_t _needs_more;
};

class EndOfMessageException {
};

#endif // INPUTMESSAGE_H
