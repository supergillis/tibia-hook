#include <string.h>

#include "EncryptedMessage.h"
#include "DecryptedMessage.h"

EncryptedMessage::EncryptedMessage() :
		Message(), dataLength_(0), checksum_(0), needsMore_(0) {
}

EncryptedMessage::EncryptedMessage(const quint8* buffer, quint16 length) :
		Message() {
	initialize(buffer, length);
}

/**
 * To encrypt a message we first need to add some padding bytes. These are
 * filled completely random. Probably CIP is checking these padding bytes to
 * check if we're hacking... Problem for later? :p
 *
 * Then we can encrypt this message using XTEA.
 *
 * And finally we replace our buffer with the encrypted buffer and we also
 * replace our length with the new buffer length.
 */
EncryptedMessage::EncryptedMessage(const DecryptedMessage* message) :
		Message() {
	if (message->isValid()) {
		quint16 rawLength = message->rawLength();
		quint16 length = rawLength;
		if (length % 8 != 0) {
			length += 8 - (length % 8);
		}

		quint8 data[length + DATA_POSITION];
		memcpy(data + DATA_POSITION, message->rawData(), rawLength);

		if (Encryption::XTEA::encrypt(data + DATA_POSITION, length, Encryption::XTEA::TIBIA_KEY)) {
			*(quint16*) (data + HEADER_POSITION) = length + CHECKSUM_LENGTH;
			*(quint32*) (data + CHECKSUM_POSITION) = Encryption::Adler::checksum(data + DATA_POSITION, length);
			initialize(data, length + DATA_POSITION);
		}
	}
}

void EncryptedMessage::initialize(const quint8* buffer, quint16 length) {
	dataLength_ = *(quint16*) (buffer + HEADER_POSITION);
	dataLength_ -= CHECKSUM_LENGTH;
	checksum_ = *(quint32*) (buffer + CHECKSUM_POSITION);
	needsMore_ = (dataLength_ + DATA_POSITION) - length;
	Message::initialize(buffer, length);
}

bool EncryptedMessage::isValid() const {
	return dataLength_ > 0 && needsMore_ == 0;
}

quint16 EncryptedMessage::length() const {
	return dataLength_;
}

const quint8* EncryptedMessage::data() const {
	return rawData() + DATA_POSITION;
}
