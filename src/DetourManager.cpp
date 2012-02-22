#include "DetourManager.h"
#include "Hook.h"

Hook* DetourManager::hook_ = NULL;
PacketStream* DetourManager::stream_ = (PacketStream*) ADDRESS_PACKET_STREAM;

bool DetourManager::sendingToClient_ = false;

SafeQueue DetourManager::clientQueue_;
SafeQueue DetourManager::serverQueue_;

MologieDetours::Detour<loopSignature*>* DetourManager::loopDetour_;
MologieDetours::Detour<sendSignature*>* DetourManager::sendDetour_;
MologieDetours::Detour<parserSignature*>* DetourManager::parserDetour_;
MologieDetours::Detour<nextPacketSignature*>* DetourManager::nextPacketDetour_;

/**
 * This function runs in the Tibia thread.
 */
void DetourManager::initialize(Hook* hook) {
	hook_ = hook;
	loopDetour_ = new MologieDetours::Detour<loopSignature*>((loopSignature*) ADDRESS_LOOP_FUNCTION, &DetourManager::onLoop);
	sendDetour_ = new MologieDetours::Detour<sendSignature*>((sendSignature*) ADDRESS_SEND_FUNCTION, &DetourManager::onSend);
	parserDetour_ = new MologieDetours::Detour<parserSignature*>((parserSignature*) ADDRESS_PARSER_FUNCTION, &DetourManager::onParse);
	nextPacketDetour_ = new MologieDetours::Detour<nextPacketSignature*>((nextPacketSignature*) ADDRESS_NEXT_PACKET_FUNCTION, &DetourManager::onNextPacket);
}

SafeQueue* DetourManager::clientQueue() {
	return &clientQueue_;
}

SafeQueue* DetourManager::serverQueue() {
	return &serverQueue_;
}

/**
 * This function runs in the Tibia thread.
 */
void* DetourManager::onLoop() {
	if (!serverQueue_.empty()) {
		qDebug() << "onLoop serverQueue";
		// Replace send buffer with new buffer
		QByteArray buffer = serverQueue_.dequeue();
		*((quint32*) ADDRESS_SEND_BUFFER_LENGTH) = buffer.length() + 8;
		memset((quint8*) ADDRESS_SEND_BUFFER, 0, 8);
		memcpy((quint8*) ADDRESS_SEND_BUFFER + 8, buffer.constData(), buffer.length());
		// Call send function with modified buffer
		sendDetour_->GetOriginalFunction()(true);
	}
	if (!clientQueue_.empty()) {
		qDebug() << "onLoop clientQueue";
		// Backup stream data
		PacketStream recover = *stream_;
		sendingToClient_ = true;
		// Replace stream data with new stream data
		QByteArray buffer = clientQueue_.dequeue();
		stream_->buffer = (quint8*) buffer.data();
		stream_->position = 0;
		stream_->size = buffer.length();
		// Call parse function with modified stream
		parserDetour_->GetOriginalFunction()();
		// Restore stream data
		sendingToClient_ = false;
		*stream_ = recover;
	}
	return loopDetour_->GetOriginalFunction()();
}

/**
 * This function runs in the Tibia thread.
 */
void DetourManager::onSend(bool encrypt) {
	qDebug() << "onSend" << encrypt;
	if (hook_) {
		quint8* buffer = (quint8*) (ADDRESS_SEND_BUFFER + 8);
		quint32 length = *((quint32*) ADDRESS_SEND_BUFFER_LENGTH) - 8;
		QByteArray data((char*) buffer, length);
		if (!hook_->receiveFromClient(data)) {
			return;
		}
	}
	sendDetour_->GetOriginalFunction()(encrypt);
}

/**
 * This function runs in the Tibia thread.
 */
void DetourManager::onParse() {
	qDebug() << "onParse" << stream_->position << stream_->size;
	parserDetour_->GetOriginalFunction()();
}

/**
 * This function runs in the Tibia thread.
 */
int DetourManager::onNextPacket() {
	if (sendingToClient_) {
		if (stream_->position < stream_->size) {
			quint8 command = *((quint8*) (stream_->buffer + stream_->position));
			stream_->position++;
			return command;
		}
		return -1;
	}
	if (hook_) {
		int command = nextPacketDetour_->GetOriginalFunction()();
		if (command != -1) {
			qDebug() << "onNextPacket" << stream_->position << stream_->size;
			quint32 position = stream_->position - 1;
			quint32 length = stream_->size - position;
			QByteArray data((char*) (stream_->buffer + position), length);
			hook_->receiveFromServer(data);
		}
		return command;
	}
	return nextPacketDetour_->GetOriginalFunction()();
}
