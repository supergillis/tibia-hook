#ifndef READONLYPACKETPROXY_H_
#define READONLYPACKETPROXY_H_

#include <QByteArray>
#include <QObject>
#include <QString>

#include <ReadOnlyPacketInterface.h>

#include "ReadOnlyPacket.h"

class ReadOnlyPacketProxy: public ReadOnlyPacketInterface {
	Q_OBJECT
	Q_PROPERTY(quint16 length READ length)
	Q_PROPERTY(quint16 position WRITE setPosition READ position)

public:
	ReadOnlyPacketProxy(const QByteArray& data): ReadOnlyPacketInterface(), packet_(data) {}
	ReadOnlyPacketProxy(const quint8* data, quint16 length): ReadOnlyPacketInterface(), packet_(data, length) {}
	virtual ~ReadOnlyPacketProxy() {}

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

private:
	ReadOnlyPacket packet_;
};

#endif /* READONLYPACKETPROXY_H_ */
