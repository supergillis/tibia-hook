#ifndef PACKET_H_
#define PACKET_H_

#include <QObject>
#include <QByteArray>
#include <QString>

#include "DecryptedMessage.h"

#define PACKET_END_OF_FILE "reached the end of the buffer"

class Packet: public QObject {
	Q_OBJECT

public:
	Packet();
	Packet(const DecryptedMessage&);

public slots:
	bool has(quint16);

	quint8 readU8();
	quint16 readU16();
	quint32 readU32();
	quint64 readU64();
	QString readString();

private:
	QByteArray _data;
	quint16 _length;
	quint16 _position;
};

#endif /* PACKET_H_ */
