#include "DetourManager.h"
#include "Hook.h"

QObject* DetourManager::parent_ = NULL;
PacketStream* DetourManager::stream_ = (PacketStream*) ADDRESS_PACKET_STREAM;
ParserSignature* DetourManager::parserFunction_ = (ParserSignature*) ADDRESS_PARSER_FUNCTION;

bool DetourManager::sendingToClient_ = false;

DataQueue DetourManager::clientQueue_;
DataQueue DetourManager::serverQueue_;

MologieDetours::Detour<LoopSignature*>* DetourManager::loopDetour_;
MologieDetours::Detour<SendSignature*>* DetourManager::sendDetour_;
MologieDetours::Detour<NextPacketSignature*>* DetourManager::nextPacketDetour_;

/**
 * This function runs in the Tibia thread.
 */
void DetourManager::initialize(QObject* parent) {
	parent_ = parent;
	loopDetour_ = new MologieDetours::Detour<LoopSignature*>((LoopSignature*) ADDRESS_LOOP_FUNCTION, &DetourManager::onLoop);
	sendDetour_ = new MologieDetours::Detour<SendSignature*>((SendSignature*) ADDRESS_SEND_FUNCTION, &DetourManager::onSend);
	nextPacketDetour_ = new MologieDetours::Detour<NextPacketSignature*>((NextPacketSignature*) ADDRESS_NEXT_PACKET_FUNCTION, &DetourManager::onNextPacket);
}

DataQueue* DetourManager::clientQueue() {
	return &clientQueue_;
}

DataQueue* DetourManager::serverQueue() {
	return &serverQueue_;
}

/**
 * This function runs in the Tibia thread.
 */
void* DetourManager::onLoop() {
	if (!serverQueue_.empty()) {
		// Replace send buffer with new buffer
		QByteArray buffer = serverQueue_.dequeue();
		*((quint32*) ADDRESS_SEND_BUFFER_LENGTH) = buffer.length() + 8;
		memset((quint8*) ADDRESS_SEND_BUFFER, 0, 8);
		memcpy((quint8*) ADDRESS_SEND_BUFFER + 8, buffer.constData(), buffer.length());
		// Call send function with modified buffer
		sendDetour_->GetOriginalFunction()(true);
	}
	if (!clientQueue_.empty()) {
		// Backup stream data
		PacketStream recover = *stream_;
		sendingToClient_ = true;
		// Replace stream data with new stream data
		QByteArray buffer = clientQueue_.dequeue();
		stream_->buffer = (quint8*) buffer.data();
		stream_->position = 0;
		stream_->size = buffer.length();
		// Call parse function with modified stream
		parserFunction_();
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
	if (encrypt && parent_) {
		quint8* buffer = (quint8*) (ADDRESS_SEND_BUFFER + 8);
		quint32 length = *((quint32*) ADDRESS_SEND_BUFFER_LENGTH) - 8;
		QCoreApplication::postEvent(parent_, new DataEvent(DataEvent::Client, buffer, length), Qt::HighEventPriority);
		return;
	}
	sendDetour_->GetOriginalFunction()(encrypt);
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
	if (parent_) {
		int command = nextPacketDetour_->GetOriginalFunction()();
		if (command != -1) {
			quint32 position = stream_->position - 1;
			quint32 length = stream_->size - position;
			QCoreApplication::postEvent(parent_, new DataEvent(DataEvent::Server, stream_->buffer + position, length), Qt::HighEventPriority);
		}
		return command;
	}
	return nextPacketDetour_->GetOriginalFunction()();
}
