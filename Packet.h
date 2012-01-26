#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>

#include <QObject>
#include <QByteArray>
#include <QString>

class Packet: public QObject {
	Q_OBJECT

public:
	Packet();

public slots:
	QString getName() {
		return "gillis";
	}

	uint8_t readU8();
	uint16_t readU16();
	uint32_t readU32();
	uint64_t readU64();
	QString readString();

private:
	QByteArray _data;
	uint16_t _position;
};

#endif /* PACKET_H_ */
