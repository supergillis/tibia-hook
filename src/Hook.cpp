#include <stdio.h>
#include <string.h>

#include <QDebug>

#include "Hook.h"
#include "ReadOnlyPacket.h"
#include "Main.h"

static int argc_ = 0;

Hook* Hook::instance_;
QMutex Hook::mutex_;
QQueue<QByteArray> Hook::queue_;

MologieDetours::Detour<eventFunctionSignature>* Hook::eventFunctionDetour_;
MologieDetours::Detour<sendFunctionSignature>* Hook::sendFunctionDetour_;
MologieDetours::Detour<receiveFunctionSignature>* Hook::receiveFunctionDetour_;

void Hook::initialize() {
	eventFunctionDetour_ = new MologieDetours::Detour<eventFunctionSignature>((eventFunctionSignature) ADDRESS_EVENT_FUNCTION, &Hook::eventFunctionHook);
	sendFunctionDetour_ = new MologieDetours::Detour<sendFunctionSignature>((sendFunctionSignature) ADDRESS_SEND_FUNCTION, &Hook::sendFunctionHook);
	//receiveFunctionDetour_ = new MologieDetours::Detour<receiveFunctionSignature>((receiveFunctionSignature) ADDRESS_RECEIVE_FUNCTION, &Hook::receiveFunctionHook);
}

/**
 * This function runs in the Tibia thread.
 */
void* Hook::eventFunctionHook() {
	if (hasQueuedPacket()) {
		qDebug() << "hasQueuedPacket";

		QByteArray packet = dequeuePacket();
		*((quint32*) ADDRESS_SEND_BUFFER_LENGTH) = packet.length() + 8;
		memset((quint8*) ADDRESS_SEND_BUFFER, 0, 8);
		memcpy((quint8*) ADDRESS_SEND_BUFFER + 8, packet.constData(), packet.length());
		sendFunctionDetour_->GetOriginalFunction()(true);
	}
	return eventFunctionDetour_->GetOriginalFunction()();
}

/**
 * This function runs in the Tibia thread.
 */
void Hook::sendFunctionHook(bool encrypt) {
	qDebug() << "writePacketFunctionHook" << encrypt;
	Hook::instance()->receiveFromClient(encrypt);
}

/**
 * This function runs in the Tibia thread.
 */
void Hook::receiveFunctionHook() {
	qDebug() << "readPacketFunctionHook";
	Hook::instance()->receiveFromServer();
}

Hook::Hook() :
		QCoreApplication(argc_, NULL), handler_(NULL) {
}

Handler* Hook::handler() {
	return handler_;
}

void Hook::setHandler(Handler* handler) {
	handler_ = handler;
}

/**
 * This function runs in both threads. It is thread-safe.
 */
bool Hook::hasQueuedPacket() {
	QMutexLocker locker(&mutex_);
	return queue_.size() > 0;
}

/**
 * This function runs in both threads. It is thread-safe.
 */
void Hook::enqueuePacket(const QByteArray data) {
	QMutexLocker locker(&mutex_);
	queue_.enqueue(data);
}

/**
 * This function runs in both threads. It is thread-safe.
 */
QByteArray Hook::dequeuePacket() {
	QMutexLocker locker(&mutex_);
	return queue_.dequeue();
}

/**
 * This function runs in the injected thread.
 */
void Hook::sendToClient(const Packet* packet) {
}

/**
 * This function runs in the injected thread.
 */
void Hook::sendToServer(const Packet* packet) {
	Hook::enqueuePacket(QByteArray((const char*) packet->data(), packet->length()));
}

/**
 * This function runs in the Tibia thread.
 */
void Hook::receiveFromClient(bool encrypt) {
	quint8* buffer = (quint8*) (ADDRESS_SEND_BUFFER + 8);
	quint32 length = *((quint32*) ADDRESS_SEND_BUFFER_LENGTH) - 8;
	ReadOnlyPacket packet(buffer, length);
	if (handler_->receiveFromClient(&packet)) {
		// If we hook this packet, then don't send it
		return;
	}
	sendFunctionDetour_->GetOriginalFunction()(encrypt);
}

/**
 * This function runs in the Tibia thread.
 */
void Hook::receiveFromServer() {
	receiveFunctionDetour_->GetOriginalFunction()();
}
