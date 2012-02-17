#ifndef HOOK_H
#define HOOK_H

#include <QCoreApplication>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>

#include "Handler.h"

#include "detours.h"

#ifdef WIN32
#else
#define ADDRESS_EVENT_FUNCTION 0x8261e70

#define ADDRESS_SEND_BUFFER 0x85d3b60
#define ADDRESS_SEND_BUFFER_LENGTH 0x85d4368
#define ADDRESS_SEND_FUNCTION 0x08264920

#define ADDRESS_RECEIVE_FUNCTION 0
#endif

typedef void* (*eventFunctionSignature)();
typedef void (*sendFunctionSignature)( bool);
typedef void (*receiveFunctionSignature)();

class Hook: public QCoreApplication {
	Q_OBJECT

public:
	static Hook* instance() {
		if(instance_ == NULL) {
			initialize();
			instance_ = new Hook();
		}
		return instance_;
	}

	Handler* handler();
	void setHandler(Handler*);

	void sendToClient(const Packet*);
	void sendToServer(const Packet*);

	void receiveFromClient(bool);
	void receiveFromServer();

private:
	Hook();
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	Handler* handler_;

	static Hook* instance_;

	static void initialize();

	static QMutex mutex_;
	static QQueue<QByteArray> queue_;
	static bool hasQueuedPacket();
	static void enqueuePacket(const QByteArray);
	static QByteArray dequeuePacket();

	static void* eventFunctionHook();
	static void sendFunctionHook(bool);
	static void receiveFunctionHook();

	static MologieDetours::Detour<eventFunctionSignature>* eventFunctionDetour_;
	static MologieDetours::Detour<sendFunctionSignature>* sendFunctionDetour_;
	static MologieDetours::Detour<receiveFunctionSignature>* receiveFunctionDetour_;
};

#endif
