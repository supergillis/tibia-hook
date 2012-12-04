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

#include <QtGlobal>

#include "DataQueue.h"
#include "Memory.h"

#ifdef Q_WS_WIN

#include <windows.h>

#define LOOP_FUNCTION_RETURN_TYPE BOOL WINAPI
#define LOOP_FUNCTION_RETURN(value) return value
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

#else

#define LOOP_FUNCTION_ADDRESS 0x0804c8f4 // XPending
#define LOOP_FUNCTION_RETURN_TYPE int
#define LOOP_FUNCTION_RETURN(value) return value
#define LOOP_FUNCTION_ARG_NAME1 display
#define LOOP_FUNCTION_ARGUMENTS LOOP_FUNCTION_ARG_NAME1
#define LOOP_FUNCTION_PARAMETERS void* LOOP_FUNCTION_ARG_NAME1

#endif

namespace MologieDetours {
template <typename function_type> class Detour;
}

class BufferHandler {
public:
    virtual void handle(const char* buffer, quint32 length) = 0;
};

class Hook;
class DetourManager {
    typedef LOOP_FUNCTION_RETURN_TYPE LoopSignature(LOOP_FUNCTION_PARAMETERS);

    typedef void OutgoingFunctionSignature(bool);
    typedef void IncomingFunctionSignature();
    typedef int IncomingNextFunctionSignature();

    struct ParseStream {
        quint8* buffer;
        quint32 size;
        quint32 position;
    } ((packed));

public:
    struct Addresses {
        MemoryLocation inFunction;
        MemoryLocation inNextFunction;
        MemoryLocation inStream;

        MemoryLocation outFunction;
        MemoryLocation outBufferLength;
        MemoryLocation outBuffer;
    };

    static void install(const Addresses& settings);
    static void uninstall();

    static void setClientBufferHandler(BufferHandler* clientHandler) {
        clientHandler_ = clientHandler;
    }

    static void setServerBufferHandler(BufferHandler* serverHandler) {
        serverHandler_ = serverHandler;
    }

    static inline DataQueue* clientQueue() {
        return &clientQueue_;
    }

    static inline DataQueue* serverQueue() {
        return &serverQueue_;
    }

private:
    DetourManager();
    ~DetourManager();

    static BufferHandler* clientHandler_;
    static BufferHandler* serverHandler_;

    static bool sendingToClient_;
    static DataQueue clientQueue_;
    static DataQueue serverQueue_;

    static MologieDetours::Detour<LoopSignature*>* loopDetour_;

    static MologieDetours::Detour<IncomingNextFunctionSignature*>* inNextFunctionDetour_;
    static IncomingFunctionSignature* inFunction_;
    static ParseStream* inStream_;

    static MologieDetours::Detour<OutgoingFunctionSignature*>* outFunctionDetour_;
    static quint32* outBufferLength_;
    static quint8* outBufferPacketChecksum_;
    static quint8* outBufferPacketData_;

    static LOOP_FUNCTION_RETURN_TYPE onLoop(LOOP_FUNCTION_PARAMETERS);
    static void onOutgoing(bool);
    static int onIncomingNext();
};

#endif
