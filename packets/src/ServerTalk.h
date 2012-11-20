#ifndef PACKETS_SERVER_TALK_H
#define PACKETS_SERVER_TALK_H

#include <stdexcept>

#include <Constants.h>
#include <Serialization.h>

using namespace constants;

namespace packets {
namespace server {

class Talk {
public:
    quint8 mode;

    quint32 channelStatementId;
    quint16 channelId;

    quint16 creatureLevel;
    QString creatureName;
    QString message;

    Position position;

    void serialization(s11n::Container& container) {
        s11n::serialization(this->channelStatementId, container);
        s11n::serialization(this->creatureName, container);
        s11n::serialization(this->creatureLevel, container);
        s11n::serialization(this->mode, container);

        switch (this->mode) {
        case TalkMode::Say:
        case TalkMode::Whisper:
        case TalkMode::Yell:
        case TalkMode::MonsterSay:
        case TalkMode::MonsterYell:
        case TalkMode::NpcTo:
        case TalkMode::NpcFrom:
        case TalkMode::BarkLow:
        case TalkMode::BarkLoud:
        case TalkMode::Spell:
            s11n::serialization(this->position, container);
            break;
        case TalkMode::Channel:
        case TalkMode::ChannelManagement:
        case TalkMode::ChannelHighlight:
        case TalkMode::GamemasterChannel:
            s11n::serialization(this->channelId, container);
            break;
        case TalkMode::PrivateFrom:
        case TalkMode::GamemasterBroadcast:
        case TalkMode::GamemasterPrivateFrom:
            // Do nothing
            break;
        case TalkMode::RVRChannel:
            container.skip(4);
            break;
        default:
            throw std::runtime_error("unknown message mode");
        }
    }
};

}
}

#endif
