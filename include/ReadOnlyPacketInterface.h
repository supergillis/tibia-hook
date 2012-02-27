#ifndef READONLYPACKETINTERFACE_H_
#define READONLYPACKETINTERFACE_H_

#include <PacketInterface.h>

class ReadOnlyPacketInterface: public PacketInterface {
public:
	ReadOnlyPacketInterface(QObject* parent = 0): PacketInterface(parent) {}
	virtual ~ReadOnlyPacketInterface() {}
};

#endif
