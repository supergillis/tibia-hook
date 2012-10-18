#ifndef SERVER_TALK_H
#define SERVER_TALK_H

#include <QString>

#include <Position.h>

#include "Constants.h"
#include "Serializeable.h"
#include "Serializer.h"

using namespace constants;

namespace packets {

namespace server {

struct TalkSerializer;

class Talk: public SerializeableBase<Talk, TalkSerializer> {
public:
    quint8 mode;

    quint32 channelStatementId;
    quint16 channelId;

    quint16 creatureLevel;
    QString creatureName;
    QString message;

    Position position;
};

struct TalkSerializer: s_typed<TalkSerializer, Talk> {
    static const QString ERROR_UNKNOWN_MESSAGE_MODE;

    typedef ss_sequence(
            ss_member(Talk::channelStatementId),
            ss_member(Talk::creatureName),
            ss_member(Talk::creatureLevel),
            ss_member(Talk::mode),
            ss_switch(Talk::mode,
                ss_case(
                    ss_in(TalkMode::Say, TalkMode::Whisper, TalkMode::Yell, TalkMode::MonsterSay, TalkMode::MonsterYell, TalkMode::NpcTo, TalkMode::NpcFrom, TalkMode::BarkLow, TalkMode::BarkLoud, TalkMode::Spell),
                    ss_member(Talk::position)),
                ss_case(
                    ss_in(TalkMode::Channel, TalkMode::ChannelManagement, TalkMode::ChannelHighlight, TalkMode::GamemasterChannel),
                    ss_member(Talk::channelId)),
                ss_case(
                    ss_in(TalkMode::PrivateFrom, TalkMode::GamemasterBroadcast, TalkMode::GamemasterPrivateFrom),
                    ss_nop),
                ss_case(
                    ss_in(TalkMode::RVRChannel),
                    ss_skip(4)),
                ss_default(
                    ss_message_exception(ERROR_UNKNOWN_MESSAGE_MODE))),
            ss_member(Talk::message)) structure;
};

const QString TalkSerializer::ERROR_UNKNOWN_MESSAGE_MODE("unknown message mode");

}

}

#endif
