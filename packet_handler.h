#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "message.h"

enum SpeakClasses {
	SPEAK_SAY = 0x01, //normal talk
	SPEAK_WHISPER = 0x02, //whispering - #w text
	SPEAK_YELL = 0x03, //yelling - #y text
	SPEAK_PRIVATE_PN = 0x04, //Player-to-NPC speaking(NPCs channel)
	SPEAK_PRIVATE_NP = 0x05, //NPC-to-Player speaking
	SPEAK_PRIVATE = 0x06, //Players speaking privately to players
	SPEAK_CHANNEL_Y = 0x07, //Yellow message in chat
	SPEAK_CHANNEL_W = 0x08, //White message in chat
	SPEAK_RVR_CHANNEL = 0x09, //Reporting rule violation - Ctrl+R
	SPEAK_RVR_ANSWER = 0x0A, //Answering report
	SPEAK_RVR_CONTINUE = 0x0B, //Answering the answer of the report
	SPEAK_BROADCAST = 0x0C, //Broadcast a message - #b
	SPEAK_CHANNEL_R1 = 0x0D, //Talk red on chat - #c
	SPEAK_PRIVATE_RED = 0x0E, //Red private - @name@ text
	SPEAK_CHANNEL_O = 0x0F, //Talk orange on text
	//SPEAK_                = 0x10, //?
	SPEAK_CHANNEL_R2 = 0x11, //Talk red anonymously on chat - #d
	//SPEAK_                = 0x12, //?
	SPEAK_MONSTER_SAY = 0x13, //Talk orange
	SPEAK_MONSTER_YELL = 0x14
//Yell orange
};

class Hook;
class PacketHandler;

class PacketHandler
{
public:
	PacketHandler(Hook*);
	virtual bool parse(Message& msg) = 0;
	virtual bool parseUnknownPacket(unsigned char, Message& msg);
protected:
	Hook* _hook;
};

class IncomingPacketHandler: public PacketHandler
{
public:
	IncomingPacketHandler(Hook*);

	virtual bool parse(Message& msg);
protected:
	virtual bool parseAddSelf(Message& msg);
	virtual bool parseMapDescription(Message& msg);
	virtual bool parseTextMessage(Message& msg);
	virtual bool parseCreatureHealth(Message& msg);
	virtual bool parseCreatureMove(Message& msg);
	virtual bool parseCreatureSpeak(Message& msg);
	virtual bool parseCreatureMoveNorth(Message& msg);
	virtual bool parseCreatureMoveEast(Message& msg);
	virtual bool parseCreatureMoveSouth(Message& msg);
	virtual bool parseCreatureMoveWest(Message& msg);
	virtual bool parsePlayerStats(Message& msg);
	virtual bool parsePing(Message& msg);
};

class OutgoingPacketHandler: public PacketHandler
{
public:
	OutgoingPacketHandler(Hook*);

	virtual bool parse(Message& msg);
protected:
	virtual bool parseUnknownPacket(unsigned char, Message& msg);
	virtual bool parseLogout(Message& msg);
	virtual bool parsePing(Message& msg);
	virtual bool parseSay(Message& msg);
};

#endif
