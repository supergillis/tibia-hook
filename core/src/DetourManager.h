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
#define LOOP_FUNCTION_RETURN_TYPE void
#define LOOP_FUNCTION_RETURN(value) value
#define LOOP_FUNCTION_ARG_NAME1 lpMsg
#define LOOP_FUNCTION_ARG_NAME2 hWnd
#define LOOP_FUNCTION_ARG_NAME3 wMsgFilterMin
#define LOOP_FUNCTION_ARG_NAME4 wMsgFilterMax
#define LOOP_FUNCTION_ARG_NAME5 wRemoveMsg
#define LOOP_FUNCTION_ARGUMENTS \
    LOOP_FUNCTION_ARG_NAME1, \
    LOOP_FUNCTION_ARG_NAME2, \
    LOOP_FUNCTION_ARG_NAME3, \
    LOOP_FUNCTION_ARG_NAME4, \
    LOOP_FUNCTION_ARG_NAME5
#define LOOP_FUNCTION_PARAMETERS \
    LPMSG LOOP_FUNCTION_ARG_NAME1, \
    HWND LOOP_FUNCTION_ARG_NAME2, \
    UINT LOOP_FUNCTION_ARG_NAME3, \
    UINT LOOP_FUNCTION_ARG_NAME4, \
    UINT LOOP_FUNCTION_ARG_NAME5

#define SEND_BUFFER_ADDRESS 0x7b9d78
#define SEND_BUFFER_LENGTH_ADDRESS 0x9d4588
#define SEND_FUNCTION_ADDRESS 0x517df0

#define PARSER_STREAM_ADDRESS 0x9d4574
#define PARSER_FUNCTION_ADDRESS 0x466270
#define PARSER_NEXT_FUNCTION_ADDRESS 0x82ff4a0
#else
/* Tibia 9.63 Adresses */
#define LOOP_FUNCTION_ADDRESS 0x0804c8f4 // XPending
#define LOOP_FUNCTION_RETURN_TYPE int
#define LOOP_FUNCTION_RETURN(value) return value
#define LOOP_FUNCTION_ARG_NAME1 display
#define LOOP_FUNCTION_ARGUMENTS LOOP_FUNCTION_ARG_NAME1
#define LOOP_FUNCTION_PARAMETERS Display* LOOP_FUNCTION_ARG_NAME1

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
    typedef LOOP_FUNCTION_RETURN_TYPE LoopSignature(LOOP_FUNCTION_PARAMETERS);

    typedef void SendSignature(bool);
    typedef void ParserSignature();
    typedef int ParserNextSignature();

    struct ParserStream {
        quint8* buffer;
        quint32 size;
        quint32 position;
    } ((packed));

public:
    inline static DetourManager* instance() {
        if(instance_ == NULL) {
            instance_ = new DetourManager();
            construct();
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

    DataQueue clientQueue_;
    DataQueue serverQueue_;
    bool sendingToClient_;

    BufferHandler* clientHandler_;
    BufferHandler* serverHandler_;

    static DetourManager* instance_;

    static void construct() {
#ifdef Q_OS_WIN
        HMODULE user32 = ::GetModuleHandle("User32.dll");
        FARPROC peekMessage = ::GetProcAddress(user32, "PeekMessageA");

        // Detour the peekMessage function
        loopDetour_ = new MologieDetours::Detour<LoopSignature*>((LoopSignature*) peekMessage, &DetourManager::onLoop);
#else
        loopDetour_ = new MologieDetours::Detour<LoopSignature*>((LoopSignature*) LOOP_FUNCTION_ADDRESS, &DetourManager::onLoop);
#endif

        // Don't detour send and parse for now
        return;

        sendDetour_ = new MologieDetours::Detour<SendSignature*>((SendSignature*) SEND_FUNCTION_ADDRESS, &DetourManager::onSend);
        parserNextDetour_ = new MologieDetours::Detour<ParserNextSignature*>((ParserNextSignature*) PARSER_NEXT_FUNCTION_ADDRESS, &DetourManager::onParserNext);
    }

    static void destruct() {
        delete parserNextDetour_;
        delete sendDetour_;
        delete loopDetour_;
    }

    static LOOP_FUNCTION_RETURN_TYPE onLoop(LOOP_FUNCTION_PARAMETERS);

    static void onSend(bool);
    static int onParserNext();

    static MologieDetours::Detour<LoopSignature*>* loopDetour_;
    static MologieDetours::Detour<SendSignature*>* sendDetour_;
    static MologieDetours::Detour<ParserNextSignature*>* parserNextDetour_;

    static ParserStream* parserStream_;
    static ParserSignature* parserFunction_;
};

#endif
