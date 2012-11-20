#ifndef PACKETS_SERVER_PLAYER_SKILLS_H
#define PACKETS_SERVER_PLAYER_SKILLS_H

#include <Serialization.h>

namespace packets {
namespace server {

class PlayerSkill {
public:
    quint8 level;
    quint8 percent;

    void serialization(s11n::Container& container) {
        s11n::serialization(this->level, container);
        s11n::serialization(this->percent, container);
    }
};

class PlayerSkills {
public:
    PlayerSkill fist;
    PlayerSkill club;
    PlayerSkill sword;
    PlayerSkill axe;
    PlayerSkill distance;
    PlayerSkill shielding;
    PlayerSkill fishing;

    void serialization(s11n::Container& container) {
        s11n::serialization(this->fist, container);
        s11n::serialization(this->club, container);
        s11n::serialization(this->sword, container);
        s11n::serialization(this->axe, container);
        s11n::serialization(this->distance, container);
        s11n::serialization(this->shielding, container);
        s11n::serialization(this->fishing, container);
    }
};

}
}

#endif
