#ifndef DECRYPTEDMESSAGE_H_
#define DECRYPTEDMESSAGE_H_

#include <stdint.h>

class Message;
class Packet {
	static const int HEADER_POSITION = 0;
	static const int HEADER_LENGTH = 2;
	static const int DATA_POSITION = HEADER_POSITION + HEADER_LENGTH;

public:
	Packet();
	Packet(const uint8_t* buffer, uint16_t length);
	Packet(const Packet&);
	~Packet();

	bool isValid() const;

	uint16_t length() const;
	const uint8_t* data() const;

	uint16_t rawLength() const;
	const uint8_t* rawData() const;

	static Packet decrypt(const Message&, const uint32_t []);

private:
	void operator=(const Packet&);

	uint8_t* _raw;
	uint16_t _rawLength;
	uint16_t _dataLength;
};

#endif /* DECRYPTEDMESSAGE_H_ */
