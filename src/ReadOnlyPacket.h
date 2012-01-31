#ifndef READONLYPACKET_H_
#define READONLYPACKET_H_

#include <QObject>

#include "Packet.h"
#include "DecryptedMessage.h"

class ReadOnlyPacket: public Packet {
	Q_OBJECT
	Q_PROPERTY(quint16 length READ length)

public:
	ReadOnlyPacket();
	ReadOnlyPacket(const DecryptedMessage*);

	const DecryptedMessage* message() const;

	quint16 length() const;
	const quint8* data() const;

private:
	DecryptedMessage message_;
};

#endif /* READONLYPACKET_H_ */
