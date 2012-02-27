#ifndef READWRITEPACKET_H_
#define READWRITEPACKET_H_

#include <QByteArray>
#include <QObject>
#include <QString>

#include <ReadWritePacketInterface.h>

#include "Packet.h"

class ReadWritePacket: public Packet {
public:
	ReadWritePacket();

	quint16 length() const;
	const quint8* data() const;

	void writeU8(quint8);
	void writeU16(quint16);
	void writeU32(quint32);
	void writeU64(quint64);
	void writeString(const QString&);

private:
	void reserve(quint16);

	QByteArray data_;
	quint16 length_;
};

#endif /* READWRITEPACKET_H_ */
