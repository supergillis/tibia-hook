#ifndef DETOURMANAGER_H_
#define DETOURMANAGER_H_

#include <detours.h>

#include "DataEvent.h"
#include "DataQueue.h"

#ifdef Q_WS_WIN
#else
#define ADDRESS_LOOP_FUNCTION 0x8261e70

#define ADDRESS_SEND_BUFFER 0x85d3b60
#define ADDRESS_SEND_BUFFER_LENGTH 0x85d4368
#define ADDRESS_SEND_FUNCTION 0x08264920

#define ADDRESS_PACKET_STREAM 0x85d8390
#define ADDRESS_PARSER_FUNCTION 0x81320f0
#define ADDRESS_NEXT_PACKET_FUNCTION 0x82703c0
#endif

class Hook;
class DetourManager {
	struct PacketStream {
		quint8* buffer;
		quint32 size;
		quint32 position;
	};

	typedef void LoopSignature();
	typedef void SendSignature( bool);
	typedef void ParserSignature();
	typedef int NextPacketSignature();

public:
	static void initialize(QObject*);

	static DataQueue* clientQueue();
	static DataQueue* serverQueue();

private:
	DetourManager();

	static QObject* parent_;
	static PacketStream* stream_;
	static ParserSignature* parserFunction_;

	static bool sendingToClient_;

	static DataQueue clientQueue_;
	static DataQueue serverQueue_;

	static void onLoop();
	static void onSend(bool);
	static int onNextPacket();

	static MologieDetours::Detour<LoopSignature*>* loopDetour_;
	static MologieDetours::Detour<SendSignature*>* sendDetour_;
	static MologieDetours::Detour<NextPacketSignature*>* nextPacketDetour_;
};

#endif /* DETOURMANAGER_H_ */
