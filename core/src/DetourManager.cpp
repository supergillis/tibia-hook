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

#include <detours.h>

#include "DetourManager.h"

void DetourManager::install(const Addresses& addresses) {
#ifdef Q_OS_WIN
    HMODULE user32 = ::GetModuleHandle("User32.dll");
    FARPROC peekMessage = ::GetProcAddress(user32, "PeekMessageA");

    // Detour the peekMessage function
    loopDetour_ = new MologieDetours::Detour<LoopSignature*>((LoopSignature*) peekMessage, &DetourManager::onLoop);
#else
    loopDetour_ = new MologieDetours::Detour<LoopSignature*>((LoopSignature*) LOOP_FUNCTION_ADDRESS, &DetourManager::onLoop);
#endif

    inNextFunctionDetour_ = new MologieDetours::Detour<IncomingNextFunctionSignature*>((IncomingNextFunctionSignature*) Memory::staticRebase(addresses.inNextFunction), &DetourManager::onIncomingNext);
    inFunction_ = (IncomingFunctionSignature*) Memory::staticRebase(addresses.inFunction);
    inStream_ = (ParseStream*) Memory::staticRebase(addresses.inStream);

    outFunctionDetour_ = new MologieDetours::Detour<OutgoingFunctionSignature*>((OutgoingFunctionSignature*) Memory::staticRebase(addresses.outFunction), &DetourManager::onOutgoing);
    outBufferLength_ = (quint32*) Memory::staticRebase(addresses.outBufferLength);
    outBufferPacketChecksum_ = (quint8*) Memory::staticRebase(addresses.outBuffer);
    outBufferPacketData_ = (quint8*) (Memory::staticRebase(addresses.outBuffer) + 8);
}

void DetourManager::uninstall() {
    delete inNextFunctionDetour_;
    delete outFunctionDetour_;
    delete loopDetour_;
}

/**
  * This function gets called in the Tibia thread.
  */
LOOP_FUNCTION_RETURN_TYPE DetourManager::onLoop(LOOP_FUNCTION_PARAMETERS) {
    if (!serverQueue_.empty()) {
        // Replace outgoing buffer with new buffer
        QByteArray buffer = serverQueue_.dequeue();
        *outBufferLength_ = buffer.length() + 8;
        memset(outBufferPacketChecksum_, 0, 8);
        memcpy(outBufferPacketData_, buffer.constData(), buffer.length());

        // Call outgoing function with modified buffer
        outFunctionDetour_->GetOriginalFunction()(true);
    }
    if (!clientQueue_.empty()) {
        // Backup stream data
        ParseStream recover = *inStream_;
        sendingToClient_ = true;

        // Replace stream data with new stream data
        QByteArray buffer = clientQueue_.dequeue();
        inStream_->buffer = (quint8*) buffer.data();
        inStream_->position = 0;
        inStream_->size = buffer.length();

        // Call parse function with modified stream
        inFunction_();

        // Restore stream data
        sendingToClient_ = false;
        *inStream_ = recover;
    }

    LOOP_FUNCTION_RETURN(loopDetour_->GetOriginalFunction()(LOOP_FUNCTION_ARGUMENTS));
}

/**
  * This function gets called in the Tibia thread.
  */
int DetourManager::onIncomingNext() {
    if (!sendingToClient_ && serverHandler_ != NULL) {
        int command = inNextFunctionDetour_->GetOriginalFunction()();
        if (command != -1) {
            ParseStream* stream = inStream_;
            quint32 position = stream->position - 1;
            quint32 length = stream->size - position;

            serverHandler_->handle((const char*) (stream->buffer + position), length);
        }
        return command;
    }
    return inNextFunctionDetour_->GetOriginalFunction()();
}

/**
  * This function gets called in the Tibia thread.
  */
void DetourManager::onOutgoing(bool encrypt) {
    if (encrypt && clientHandler_ != NULL) {
        clientHandler_->handle((const char*) outBufferPacketData_, (*outBufferLength_) - 8);
        return;
    }
    outFunctionDetour_->GetOriginalFunction()(encrypt);
}

/* Initialize static variables */

BufferHandler* DetourManager::clientHandler_ = NULL;
BufferHandler* DetourManager::serverHandler_ = NULL;

bool DetourManager::sendingToClient_ = false;
DataQueue DetourManager::clientQueue_;
DataQueue DetourManager::serverQueue_;

MologieDetours::Detour<DetourManager::LoopSignature*>* DetourManager::loopDetour_;

MologieDetours::Detour<DetourManager::IncomingNextFunctionSignature*>* DetourManager::inNextFunctionDetour_;
DetourManager::IncomingFunctionSignature* DetourManager::inFunction_ = NULL;
DetourManager::ParseStream* DetourManager::inStream_ = NULL;

MologieDetours::Detour<DetourManager::OutgoingFunctionSignature*>* DetourManager::outFunctionDetour_;
quint32* DetourManager::outBufferLength_ = NULL;
quint8* DetourManager::outBufferPacketChecksum_ = NULL;
quint8* DetourManager::outBufferPacketData_ = NULL;
