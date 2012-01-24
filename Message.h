#ifndef ENCRYPTEDMESSAGE_H_
#define ENCRYPTEDMESSAGE_H_

#include <stdint.h>

class Packet;
class Message {
	static const int HEADER_POSITION = 0;
	static const int HEADER_LENGTH = 2;
	static const int CHECKSUM_POSITION = HEADER_POSITION + HEADER_LENGTH;
	static const int CHECKSUM_LENGTH = 4;
	static const int DATA_POSITION = CHECKSUM_POSITION + CHECKSUM_LENGTH;

public:
	Message();
	Message(const uint8_t*, uint16_t);
	Message(const Message&);
	~Message();

	bool isValid() const;

	uint16_t length() const;
	const uint8_t* data() const;

	uint16_t rawLength() const;
	const uint8_t* rawData() const;

	static Message encrypt(const Packet&, const uint32_t []);

private:
	void operator=(const Message&);

	uint8_t* _raw;
	uint16_t _rawLength;
	uint16_t _dataLength;
	uint32_t _checksum;
	uint16_t _needsMore;
};

#endif /* ENCRYPTEDMESSAGE_H_ */
