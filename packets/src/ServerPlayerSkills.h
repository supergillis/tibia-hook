#ifndef SERVER_PLAYERSKILLS_H
#define SERVER_PLAYERSKILLS_H

#include "Serializeable.h"
#include "Serializer.h"

namespace packets {

namespace server {

struct PlayerSkillSerializer;
struct PlayerSkillsSerializer;

class PlayerSkill: public SerializeableBase<PlayerSkill, PlayerSkillSerializer> {
public:
    quint8 level;
    quint8 percent;
};

class PlayerSkills: public SerializeableBase<PlayerSkills, PlayerSkillsSerializer> {
public:
    enum Type {
        Fist = 0,
        Club,
        Sword,
        Axe,
        Distance,
        Shielding,
        Fishing,
        LastSkill
    };

    PlayerSkills(): skills(LastSkill) {}

    QVector<PlayerSkill> skills;
};

struct PlayerSkillSerializer: s_typed<PlayerSkillSerializer, PlayerSkill> {
    typedef ss_sequence(
            ss_member(PlayerSkill::level),
            ss_member(PlayerSkill::percent)) structure;
};

struct PlayerSkillsSerializer: s_typed<PlayerSkillsSerializer, PlayerSkills> {
    typedef ss_repeat(PlayerSkills::LastSkill, PlayerSkillSerializer, PlayerSkills::skills) structure;
};

}

}

#endif
