#ifndef PACKETINTERFACE_H_
#define PACKETINTERFACE_H_

#include <QtGlobal>
#include <QObject>
#include <QString>

class PacketInterface: public QObject {
public:
	PacketInterface(QObject* parent = 0): QObject(parent) {}
	virtual ~PacketInterface() {}

	virtual quint16 length() const = 0;
	virtual const quint8* data() const = 0;

	virtual quint16 position() const = 0;
	virtual void setPosition(quint16) = 0;

	virtual void skip(quint16) = 0;
	virtual bool has(quint16) const = 0;

	virtual quint8 readU8() = 0;
	virtual quint16 readU16() = 0;
	virtual quint32 readU32() = 0;
	virtual quint64 readU64() = 0;
	virtual QString readString() = 0;
};

#endif
