#ifndef DECRYPTEDMESSAGE_H_
#define DECRYPTEDMESSAGE_H_

#include <stdint.h>

#include <QByteArray>

#include "Message.h"

class EncryptedMessage;
class DecryptedMessage: public Message {
	static const int HEADER_POSITION = 0;
	static const int HEADER_LENGTH = 2;
	static const int DATA_POSITION = HEADER_POSITION + HEADER_LENGTH;

public:
	DecryptedMessage();
	DecryptedMessage(const uint8_t*, uint16_t);

	bool isValid() const;

	uint16_t length() const;
	const uint8_t* data() const;

	static DecryptedMessage decrypt(const EncryptedMessage&, const uint32_t []);

private:
	uint16_t _dataLength;
};

#endif /* DECRYPTEDMESSAGE_H_ */
