#ifndef READWRITEPACKET_H_
#define READWRITEPACKET_H_

#include <QObject>
#include <QByteArray>

#include "Packet.h"

class ReadWritePacket: public Packet {
	Q_OBJECT

public:
	ReadWritePacket();

public slots:
	quint16 length() const;
	const quint8* data() const;

	void writeU8(quint8);
	void writeU16(quint16);
	void writeU32(quint32);
	void writeU64(quint64);
	void writeString(const QString&);

private:
	void reserve(quint16);

	QByteArray _data;
	quint16 _length;
};

#endif /* READWRITEPACKET_H_ */
