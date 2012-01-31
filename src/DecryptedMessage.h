#ifndef DECRYPTEDMESSAGE_H_
#define DECRYPTEDMESSAGE_H_

#include <QByteArray>

#include "Message.h"
#include "Encryption.h"
#include "Packet.h"

class EncryptedMessage;
class DecryptedMessage: public Message {
	static const int HEADER_POSITION = 0;
	static const int HEADER_LENGTH = 2;
	static const int DATA_POSITION = HEADER_POSITION + HEADER_LENGTH;

public:
	DecryptedMessage();
	DecryptedMessage(const quint8*, quint16);
	DecryptedMessage(const Packet*);
	DecryptedMessage(const EncryptedMessage*);

	bool isValid() const;

	quint16 length() const;
	const quint8* data() const;

protected:
	void initialize(const quint8*, quint16);

private:
	quint16 dataLength_;
};

#endif /* DECRYPTEDMESSAGE_H_ */
