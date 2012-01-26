#ifndef DECRYPTEDMESSAGE_H_
#define DECRYPTEDMESSAGE_H_

#include <QByteArray>

#include "Message.h"
#include "Encryption.h"

class EncryptedMessage;
class DecryptedMessage: public Message {
	static const int HEADER_POSITION = 0;
	static const int HEADER_LENGTH = 2;
	static const int DATA_POSITION = HEADER_POSITION + HEADER_LENGTH;

public:
	DecryptedMessage();
	DecryptedMessage(const quint8*, quint16);

	bool isValid() const;

	quint16 length() const;
	const quint8* data() const;

	static DecryptedMessage decrypt(const EncryptedMessage&, const quint32 []);

private:
	quint16 _dataLength;
};

#endif /* DECRYPTEDMESSAGE_H_ */
