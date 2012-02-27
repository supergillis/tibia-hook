#ifndef READWRITEPACKETINTERFACE_H_
#define READWRITEPACKETINTERFACE_H_

#include <PacketInterface.h>

class ReadWritePacketInterface: public PacketInterface {
public:
	ReadWritePacketInterface(QObject* parent = 0): PacketInterface(parent) {}
	virtual ~ReadWritePacketInterface() {}

	virtual void writeU8(quint8) = 0;
	virtual void writeU16(quint16) = 0;
	virtual void writeU32(quint32) = 0;
	virtual void writeU64(quint64) = 0;
	virtual void writeString(const QString&) = 0;
};

#endif
