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

#include <QDebug>
#include <QObject>

#include <detours.h>

#include "DataQueue.h"

#ifdef Q_WS_WIN
#else
/* Tibia 9.61 Adresses */
#define LOOP_FUNCTION_ADDRESS 0x804c874 // XNextEvent
#define LOOP_FUNCTION_ARG_NAME1 one
#define LOOP_FUNCTION_ARG_NAME2 two
#define LOOP_FUNCTION_ARGUMENTS LOOP_FUNCTION_ARG_NAME1, LOOP_FUNCTION_ARG_NAME2
#define LOOP_FUNCTION_PARAMETERS void* LOOP_FUNCTION_ARG_NAME1, void* LOOP_FUNCTION_ARG_NAME2

#define SEND_BUFFER_ADDRESS 0x85d6960
#define SEND_BUFFER_LENGTH_ADDRESS 0x85d7168
#define SEND_FUNCTION_ADDRESS 0x82f5e30

#define PARSER_STREAM_ADDRESS 0x85db190
#define PARSER_FUNCTION_ADDRESS 0x814aa40
#define PARSER_NEXT_FUNCTION_ADDRESS 0x8301440
#endif

class Hook;
class DetourManager: public QObject {
    Q_OBJECT

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

signals:
    void onClientMessage(QByteArray);
    void onServerMessage(QByteArray);

protected:
    void connectNotify(const char* signal) {
        if(strcmp(signal, SIGNAL(onClientMessage(QByteArray))) == 0) {
            clientSignalConnected_++;
            if(clientSignalConnected_ > 1) {
                qWarning() << "connected more than once to onClientMessage";
            }
        }
        else if(strcmp(signal, SIGNAL(onServerMessage(QByteArray))) == 0) {
            serverSignalConnected_++;
            if(serverSignalConnected_ > 1) {
                qWarning() << "connected more than once to onServerMessage";
            }
        }
    }

    void disconnectNotify(const char* signal) {
        if(strcmp(signal, SIGNAL(onClientMessage(QByteArray))) == 0) {
            clientSignalConnected_--;
        }
        else if(strcmp(signal, SIGNAL(onServerMessage(QByteArray))) == 0) {
            serverSignalConnected_--;
        }
    }

private:
    DetourManager();

    ParserStream* parserStream_;
    ParserSignature* parserFunction_;

    DataQueue clientQueue_;
    DataQueue serverQueue_;
    bool sendingToClient_;

    int clientSignalConnected_;
    int serverSignalConnected_;

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
