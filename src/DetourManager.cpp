#include "DetourManager.h"

DetourManager* DetourManager::instance_ = NULL;

MologieDetours::Detour<DetourManager::LoopSignature*>* DetourManager::loopDetour_;
MologieDetours::Detour<DetourManager::SendSignature*>* DetourManager::sendDetour_;
MologieDetours::Detour<DetourManager::NextPacketSignature*>* DetourManager::nextPacketDetour_;

DetourManager::DetourManager():
	sendingToClient_(false),
	stream_((PacketStream*) ADDRESS_PACKET_STREAM),
	parserFunction_((ParserSignature*) ADDRESS_PARSER_FUNCTION),
	clientSignalConnected_(0),
	serverSignalConnected_(0) {
}

void DetourManager::initialize() {
	loopDetour_ = new MologieDetours::Detour<LoopSignature*>((LoopSignature*) ADDRESS_LOOP_FUNCTION, &DetourManager::onLoop);
	sendDetour_ = new MologieDetours::Detour<SendSignature*>((SendSignature*) ADDRESS_SEND_FUNCTION, &DetourManager::onSend);
	nextPacketDetour_ = new MologieDetours::Detour<NextPacketSignature*>((NextPacketSignature*) ADDRESS_NEXT_PACKET_FUNCTION, &DetourManager::onNextPacket);
}

/**
  * This function runs in the Tibia thread.
  */
void DetourManager::onLoop() {
	DataQueue* clientQueue = instance_->clientQueue();
	DataQueue* serverQueue = instance_->serverQueue();

	if (!serverQueue->empty()) {
		// Replace send buffer with new buffer
		QByteArray buffer = serverQueue->dequeue();
		*((quint32*) ADDRESS_SEND_BUFFER_LENGTH) = buffer.length() + 8;
		memset((quint8*) ADDRESS_SEND_BUFFER, 0, 8);
		memcpy((quint8*) ADDRESS_SEND_BUFFER + 8, buffer.constData(), buffer.length());
		// Call send function with modified buffer
		sendDetour_->GetOriginalFunction()(true);
	}
	if (!clientQueue->empty()) {
		// Backup stream data
		PacketStream recover = *instance_->stream_;
		instance_->sendingToClient_ = true;
		// Replace stream data with new stream data
		QByteArray buffer = clientQueue->dequeue();
		instance_->stream_->buffer = (quint8*) buffer.data();
		instance_->stream_->position = 0;
		instance_->stream_->size = buffer.length();
		// Call parse function with modified stream
		instance_->parserFunction_();
		// Restore stream data
		instance_->sendingToClient_ = false;
		*instance_->stream_ = recover;
	}

	loopDetour_->GetOriginalFunction()();
}

/**
  * This function runs in the Tibia thread.
  */
void DetourManager::onSend(bool encrypt) {
	if (encrypt && instance_->clientSignalConnected_ > 0) {
		quint8* buffer = (quint8*) (ADDRESS_SEND_BUFFER + 8);
		quint32 length = *((quint32*) ADDRESS_SEND_BUFFER_LENGTH) - 8;
		QByteArray data((char*) buffer, length);
		emit instance_->onClientMessage(data);
		//QCoreApplication::postEvent(parent_, new DataEvent(DataEvent::Client, buffer, length), Qt::HighEventPriority);
		return;
	}
	sendDetour_->GetOriginalFunction()(encrypt);
}

/**
  * This function runs in the Tibia thread.
  */
int DetourManager::onNextPacket() {
	if (instance_->sendingToClient_) {
		PacketStream* stream = instance_->stream_;
		if (stream->position < stream->size) {
			quint8 command = *((quint8*) (stream->buffer + stream->position));
			stream->position++;
			return command;
		}
		return -1;
	}
	if (instance_->serverSignalConnected_ > 0) {
		int command = nextPacketDetour_->GetOriginalFunction()();
		if (command != -1) {
			PacketStream* stream = instance_->stream_;
			quint32 position = stream->position - 1;
			quint32 length = stream->size - position;
			QByteArray data((char*) (stream->buffer + position), length);
			emit instance_->onServerMessage(data);
			//QCoreApplication::postEvent(parent_, new DataEvent(DataEvent::Server, stream_->buffer + position, length), Qt::HighEventPriority);
		}
		return command;
	}
	return nextPacketDetour_->GetOriginalFunction()();
}
