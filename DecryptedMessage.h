#ifndef DECRYPTEDMESSAGE_H_
#define DECRYPTEDMESSAGE_H_

#include <stdint.h>

class EncryptedMessage;
class DecryptedMessage {
	static const int HEADER_POSITION = 0;
	static const int HEADER_LENGTH = 2;
	static const int DATA_POSITION = HEADER_POSITION + HEADER_LENGTH;

public:
	DecryptedMessage();
	DecryptedMessage(const uint8_t* buffer, uint16_t length);
	DecryptedMessage(const DecryptedMessage&);
	~DecryptedMessage();

	bool isValid() const;

	uint16_t length() const;
	const uint8_t* data() const;

	uint16_t rawLength() const;
	const uint8_t* rawData() const;

	static DecryptedMessage decrypt(const EncryptedMessage&, const uint32_t []);

private:
	void operator=(const DecryptedMessage&);

	uint8_t* _raw;
	uint16_t _rawLength;
	uint16_t _dataLength;
};

#endif /* DECRYPTEDMESSAGE_H_ */
