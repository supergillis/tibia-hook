#include "Serializeable.h"
#include "Serializer.h"

#include "ServerPlayerSkills.h"
#include "ServerTalk.h"

#include <QDebug>

void testServerPlayerSkills();
void testServerTalk();

int main(int argc, char** argv) {
    testServerPlayerSkills();
    testServerTalk();
}

using namespace packets;

void testServerPlayerSkills() {
    qDebug() << "** testing server::PlayerSkills **";
    server::PlayerSkills packet;
    packet.skills[server::PlayerSkills::Axe].level = 30;
    packet.skills[server::PlayerSkills::Axe].percent = 14;
    packet.skills[server::PlayerSkills::Club].level = 20;
    packet.skills[server::PlayerSkills::Club].percent = 18;
    packet.skills[server::PlayerSkills::Distance].level = 10;
    packet.skills[server::PlayerSkills::Distance].percent = 40;
    packet.skills[server::PlayerSkills::Fishing].level = 28;
    packet.skills[server::PlayerSkills::Fishing].percent = 81;
    packet.skills[server::PlayerSkills::Fist].level = 10;
    packet.skills[server::PlayerSkills::Fist].percent = 1;
    packet.skills[server::PlayerSkills::Shielding].level = 80;
    packet.skills[server::PlayerSkills::Shielding].percent = 88;
    packet.skills[server::PlayerSkills::Sword].level = 81;
    packet.skills[server::PlayerSkills::Sword].percent = 97;

    PacketBuilder builder;
    packet.serialize(builder);

    PacketReader reader(builder.build());
    server::PlayerSkills deserialized;
    deserialized.deserialize(reader);

    foreach(server::PlayerSkill skill, deserialized.skills) {
        qDebug() << skill.level << skill.percent;
    }
}

void testServerTalk() {
    qDebug() << "** testing server::Talk **";

    server::Talk packet;
    packet.mode = constants::TalkMode::Say;
    packet.creatureName = "Gillis";
    packet.creatureLevel = 75;
    packet.position.x = 100;
    packet.position.y = 150;
    packet.position.z = 7;

    PacketBuilder builder;
    packet.serialize(builder);

    PacketReader reader(builder.build());
    server::Talk deserialized;
    deserialized.deserialize(reader);

    qDebug() << packet.creatureName;
    qDebug() << packet.creatureLevel;
    qDebug() << packet.position.x;
    qDebug() << packet.position.y;
    qDebug() << packet.position.z;
}
