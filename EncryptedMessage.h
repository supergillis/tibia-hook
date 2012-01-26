#ifndef ENCRYPTEDMESSAGE_H_
#define ENCRYPTEDMESSAGE_H_

#include <QByteArray>

#include "Message.h"
#include "Encryption.h"

class DecryptedMessage;
class EncryptedMessage: public Message {
	static const int HEADER_POSITION = 0;
	static const int HEADER_LENGTH = 2;
	static const int CHECKSUM_POSITION = HEADER_POSITION + HEADER_LENGTH;
	static const int CHECKSUM_LENGTH = 4;
	static const int DATA_POSITION = CHECKSUM_POSITION + CHECKSUM_LENGTH;

public:
	EncryptedMessage();
	EncryptedMessage(const quint8*, quint16);

	bool isValid() const;

	quint16 length() const;
	const quint8* data() const;

	static EncryptedMessage encrypt(const DecryptedMessage&, const quint32 []);

private:
	QByteArray _raw;
	quint16 _dataLength;
	quint32 _checksum;
	quint16 _needsMore;
};

#endif /* ENCRYPTEDMESSAGE_H_ */
