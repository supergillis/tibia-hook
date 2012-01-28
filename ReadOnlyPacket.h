#ifndef READONLYPACKET_H_
#define READONLYPACKET_H_

#include <QObject>

#include "Packet.h"
#include "DecryptedMessage.h"

class ReadOnlyPacket: public Packet {
	Q_OBJECT

public:
	ReadOnlyPacket();
	ReadOnlyPacket(const DecryptedMessage&);

	const DecryptedMessage& message() const;

public slots:
	quint16 length() const;
	const quint8* data() const;

private:
	DecryptedMessage _message;
};

#endif /* READONLYPACKET_H_ */