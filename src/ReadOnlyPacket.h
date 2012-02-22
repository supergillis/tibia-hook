#ifndef READONLYPACKET_H_
#define READONLYPACKET_H_

#include <QObject>

#include "Packet.h"

class ReadOnlyPacket: public Packet {
	Q_OBJECT
	Q_PROPERTY(quint16 length READ length)

public:
	ReadOnlyPacket(const QByteArray&);
	ReadOnlyPacket(const quint8*, quint16);

	quint16 length() const;
	const quint8* data() const;

private:
	QByteArray raw_;
};

#endif /* READONLYPACKET_H_ */
