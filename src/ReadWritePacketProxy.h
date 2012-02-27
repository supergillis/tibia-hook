#ifndef READWRITEPACKETPROXY_H_
#define READWRITEPACKETPROXY_H_

#include <QByteArray>
#include <QObject>
#include <QString>

#include <ReadWritePacketInterface.h>

#include "ReadWritePacket.h"

class ReadWritePacketProxy: public ReadWritePacketInterface {
	Q_OBJECT
	Q_PROPERTY(quint16 length READ length)
	Q_PROPERTY(quint16 position WRITE setPosition READ position)

public:
	ReadWritePacketProxy(): ReadWritePacketInterface(), packet_() {}
	virtual ~ReadWritePacketProxy() {};

	quint16 length() const {
		return packet_.length();
	}

	const quint8* data() const {
		return packet_.data();
	}

public slots:
	quint16 position() const {
		return packet_.position();
	}

	void setPosition(quint16 position) {
		packet_.setPosition(position);
	}

	void skip(quint16 length) {
		packet_.skip(length);
	}

	bool has(quint16 length) const {
		return packet_.has(length);
	}

	quint8 readU8() {
		return packet_.readU8();
	}

	quint16 readU16() {
		return packet_.readU16();
	}

	quint32 readU32() {
		return packet_.readU32();
	}

	quint64 readU64() {
		return packet_.readU64();
	}

	QString readString() {
		return packet_.readString();
	}

	void writeU8(quint8 value) {
		packet_.writeU8(value);
	}

	void writeU16(quint16 value) {
		packet_.writeU16(value);
	}

	void writeU32(quint32 value) {
		packet_.writeU32(value);
	}

	void writeU64(quint64 value) {
		packet_.writeU64(value);
	}

	void writeString(const QString& value) {
		packet_.writeString(value);
	}

private:
	ReadWritePacket packet_;
};

#endif /* READWRITEPACKETPROXY_H_ */
