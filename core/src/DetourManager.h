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

#ifndef DETOURMANAGER_H
#define DETOURMANAGER_H

#include <detours.h>

#include "DataQueue.h"

#ifdef Q_WS_WIN
#else
/* Tibia 9.63 Adresses */
#define LOOP_FUNCTION_ADDRESS 0x0804c8f4 // XPending
#define LOOP_FUNCTION_ARG_NAME1 one
#define LOOP_FUNCTION_ARGUMENTS LOOP_FUNCTION_ARG_NAME1
#define LOOP_FUNCTION_PARAMETERS void* LOOP_FUNCTION_ARG_NAME1

#define SEND_BUFFER_ADDRESS 0x85d4980
#define SEND_BUFFER_LENGTH_ADDRESS 0x85d5188
#define SEND_FUNCTION_ADDRESS 0x82f3e90

#define PARSER_STREAM_ADDRESS 0x85d91b0
#define PARSER_FUNCTION_ADDRESS 0x814c2d0
#define PARSER_NEXT_FUNCTION_ADDRESS 0x82ff4a0
#endif

class BufferHandler {
public:
    virtual void handle(const QByteArray& data) = 0;
};

class Hook;
class DetourManager {
    typedef void LoopSignature(LOOP_FUNCTION_PARAMETERS);
    typedef void SendSignature(bool);
    typedef void ParserSignature();
    typedef int ParserNextSignature();

    struct ParserStream {
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

    void setClientBufferHandler(BufferHandler*);
    void setServerBufferHandler(BufferHandler*);

private:
    DetourManager();

    ParserStream* parserStream_;
    ParserSignature* parserFunction_;

    DataQueue clientQueue_;
    DataQueue serverQueue_;
    bool sendingToClient_;

    BufferHandler* clientHandler_;
    BufferHandler* serverHandler_;

    static DetourManager* instance_;

    static void initialize() {
        loopDetour_ = new MologieDetours::Detour<LoopSignature*>((LoopSignature*) LOOP_FUNCTION_ADDRESS, &DetourManager::onLoop);
        sendDetour_ = new MologieDetours::Detour<SendSignature*>((SendSignature*) SEND_FUNCTION_ADDRESS, &DetourManager::onSend);
        parserNextDetour_ = new MologieDetours::Detour<ParserNextSignature*>((ParserNextSignature*) PARSER_NEXT_FUNCTION_ADDRESS, &DetourManager::onParserNext);
    }

    static void onLoop(LOOP_FUNCTION_PARAMETERS);
    static void onSend(bool);
    static int onParserNext();

    static MologieDetours::Detour<LoopSignature*>* loopDetour_;
    static MologieDetours::Detour<SendSignature*>* sendDetour_;
    static MologieDetours::Detour<ParserNextSignature*>* parserNextDetour_;
};

#endif
