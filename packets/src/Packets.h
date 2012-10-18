#ifndef PACKETS_H
#define PACKETS_H

#define REGISTER_SERVER_PACKETS(FUNCTION) \
    FUNCTION(161, ::packets::ServerPlayerSkills) \
    FUNCTION(170, ::packets::ServerTalk) \
#endif

#endif
