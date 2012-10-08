/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DetourManager.h"
#include "Application.h"

DetourManager* DetourManager::instance_ = NULL;

MologieDetours::Detour<DetourManager::LoopSignature*>* DetourManager::loopDetour_;
MologieDetours::Detour<DetourManager::SendSignature*>* DetourManager::sendDetour_;
MologieDetours::Detour<DetourManager::ParserNextSignature*>* DetourManager::parserNextDetour_;

DetourManager::ParserStream* DetourManager::parserStream_ = (DetourManager::ParserStream*) PARSER_STREAM_ADDRESS;
DetourManager::ParserSignature* DetourManager::parserFunction_ = (DetourManager::ParserSignature*) PARSER_FUNCTION_ADDRESS;

DetourManager::DetourManager():
    sendingToClient_(false),
    clientHandler_(NULL),
    serverHandler_(NULL) {
}

void DetourManager::setClientBufferHandler(BufferHandler* clientHandler) {
    clientHandler_ = clientHandler;
}

void DetourManager::setServerBufferHandler(BufferHandler* serverHandler) {
    serverHandler_ = serverHandler;
}

/**
  * This function runs in the Tibia thread.
  */
LOOP_FUNCTION_RETURN_TYPE DetourManager::onLoop(LOOP_FUNCTION_PARAMETERS) {
	DataQueue* clientQueue = instance_->clientQueue();
	DataQueue* serverQueue = instance_->serverQueue();

	if (!serverQueue->empty()) {
		// Replace send buffer with new buffer
        QByteArray buffer = serverQueue->dequeue();
		*((quint32*) SEND_BUFFER_LENGTH_ADDRESS) = buffer.length() + 8;
        memset((quint8*) SEND_BUFFER_ADDRESS, 0, 8);
        memcpy((quint8*) SEND_BUFFER_ADDRESS + 8, buffer.constData(), buffer.length());
		// Call send function with modified buffer
		sendDetour_->GetOriginalFunction()(true);
	}
	if (!clientQueue->empty()) {
		// Backup stream data
		ParserStream recover = *instance_->parserStream_;
		instance_->sendingToClient_ = true;
		// Replace stream data with new stream data
		QByteArray buffer = clientQueue->dequeue();
		instance_->parserStream_->buffer = (quint8*) buffer.data();
		instance_->parserStream_->position = 0;
		instance_->parserStream_->size = buffer.length();
		// Call parse function with modified stream
		instance_->parserFunction_();
		// Restore stream data
		instance_->sendingToClient_ = false;
		*instance_->parserStream_ = recover;
	}

    LOOP_FUNCTION_RETURN(loopDetour_->GetOriginalFunction()(LOOP_FUNCTION_ARGUMENTS));
}

/**
  * This function runs in the Tibia thread.
  */
void DetourManager::onSend(bool encrypt) {
    if (encrypt && instance_->clientHandler_ != NULL) {
		quint8* buffer = (quint8*) (SEND_BUFFER_ADDRESS + 8);
        quint32 length = *((quint32*) SEND_BUFFER_LENGTH_ADDRESS) - 8;

        QByteArray data((char*) buffer, length);
        instance_->clientHandler_->handle(data);
        return;
    }
	sendDetour_->GetOriginalFunction()(encrypt);
}

/**
  * This function runs in the Tibia thread.
  */
int DetourManager::onParserNext() {
    if (!instance_->sendingToClient_ && instance_->serverHandler_ != NULL) {
		int command = parserNextDetour_->GetOriginalFunction()();
		if (command != -1) {
			ParserStream* stream = instance_->parserStream_;
			quint32 position = stream->position - 1;
            quint32 length = stream->size - position;

            QByteArray data((char*) (stream->buffer + position), length);
            instance_->serverHandler_->handle(data);
		}
		return command;
    }
    return parserNextDetour_->GetOriginalFunction()();
}
