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
	ReadOnlyPacketProxy(const QByteArray&);
	ReadOnlyPacketProxy(const quint8*, quint16);

	virtual ~ReadOnlyPacketProxy() {}

	inline quint16 length() const {
		return packet_.length();
	}

	inline const quint8* data() const {
		return packet_.data();
	}

public slots:
	inline quint16 position() const {
		return packet_.position();
	}

	inline void setPosition(quint16 position) {
		packet_.setPosition(position);
	}

	inline void skip(quint16 length) {
		packet_.skip(length);
	}

	inline bool has(quint16 length) const {
		return packet_.has(length);
	}

	inline quint8 readU8() {
		return packet_.readU8();
	}

	inline quint16 readU16() {
		return packet_.readU16();
	}

	inline quint32 readU32() {
		return packet_.readU32();
	}

	inline quint64 readU64() {
		return packet_.readU64();
	}

	inline QString readString() {
		return packet_.readString();
	}

private:
	ReadOnlyPacket packet_;
};

#endif /* READONLYPACKETPROXY_H_ */
