#ifndef READONLYPACKET_H_
#define READONLYPACKET_H_

#include <QByteArray>
#include <QObject>
#include <QString>

#include "Packet.h"

class ReadOnlyPacket: public Packet {
public:
	ReadOnlyPacket(const QByteArray&);
	ReadOnlyPacket(const quint8*, quint16);

	quint16 length() const;
	const quint8* data() const;

private:
	QByteArray raw_;
};

#endif /* READONLYPACKET_H_ */
