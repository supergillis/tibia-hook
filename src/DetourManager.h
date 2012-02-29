#ifndef DETOURMANAGER_H_
#define DETOURMANAGER_H_

#include <QDebug>
#include <QObject>

#include <detours.h>

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
class DetourManager: public QObject {
	Q_OBJECT

	typedef void LoopSignature();
	typedef void SendSignature( bool);
	typedef void ParserSignature();
	typedef int NextPacketSignature();

	struct PacketStream {
		quint8* buffer;
		quint32 size;
		quint32 position;
	};

public:
	inline static DetourManager* instance() {
		if(instance_ == NULL) {
			instance_ = new DetourManager();
			initialize();
		}
		return instance_;
	}

	inline DataQueue* clientQueue() {
		return &clientQueue_;
	}

	inline DataQueue* serverQueue() {
		return &serverQueue_;
	}

signals:
	void onClientMessage(QByteArray);
	void onServerMessage(QByteArray);

protected:
	void connectNotify(const char* signal) {
		if(strcmp(signal, SIGNAL(onClientMessage(QByteArray))) == 0) {
			clientSignalConnected_++;
			if(clientSignalConnected_ > 1) {
				qWarning() << "connected more than once to onClientMessage";
			}
		}
		else if(strcmp(signal, SIGNAL(onServerMessage(QByteArray))) == 0) {
			serverSignalConnected_++;
			if(serverSignalConnected_ > 1) {
				qWarning() << "connected more than once to onServerMessage";
			}
		}
	}

	void disconnectNotify(const char* signal) {
		if(strcmp(signal, SIGNAL(onClientMessage(QByteArray))) == 0) {
			clientSignalConnected_--;
		}
		else if(strcmp(signal, SIGNAL(onServerMessage(QByteArray))) == 0) {
			serverSignalConnected_--;
		}
	}

private:
	DetourManager();

	PacketStream* stream_;
	ParserSignature* parserFunction_;

	DataQueue clientQueue_;
	DataQueue serverQueue_;
	bool sendingToClient_;

	int clientSignalConnected_;
	int serverSignalConnected_;

	static DetourManager* instance_;

	static void initialize();

	static void onLoop();
	static void onSend(bool);
	static int onNextPacket();

	static MologieDetours::Detour<LoopSignature*>* loopDetour_;
	static MologieDetours::Detour<SendSignature*>* sendDetour_;
	static MologieDetours::Detour<NextPacketSignature*>* nextPacketDetour_;
};

#endif /* DETOURMANAGER_H_ */
