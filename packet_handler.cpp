#include <iostream>
#include <list>
#include <stdio.h>

#include "packet_handler.h"
#include "hook.h"

/* This class uses function pointers in a map
 * instead of a normal switch dispatcher.
 * (too make things a bit complex :p) */

PacketHandler::PacketHandler(Hook* hook) :
		_hook(hook) {
}

bool PacketHandler::parseUnknownPacket(unsigned char type, Message& msg) {
	printf("[parseUnknownPacket] type: 0x%X, length: %d\n", type, msg.getBufferLength());
	msg.setBufferPosition(msg.getBufferLength());
	return true;
}

IncomingPacketHandler::IncomingPacketHandler(Hook* handler) :
		PacketHandler(handler) {
}

bool IncomingPacketHandler::parse(Message& msg) {
	unsigned char type = msg.getByte();
	switch (type) {
	case 0x0A:
		return parseAddSelf(msg);
	case 0x64:
		return parseMapDescription(msg);
	case 0x8C:
		return parseCreatureHealth(msg);
	case 0x6D:
		return parseCreatureMove(msg);
	case 0xB4:
		return parseTextMessage(msg);
	case 0x1E:
		return parsePing(msg);
	case 0xAA:
		return parseCreatureSpeak(msg);
	case 0x65:
		return parseCreatureMoveNorth(msg);
	case 0x66:
		return parseCreatureMoveEast(msg);
	case 0x67:
		return parseCreatureMoveSouth(msg);
	case 0x68:
		return parseCreatureMoveWest(msg);
	case 0xA0:
		return parsePlayerStats(msg);
	}
	return parseUnknownPacket(type, msg);
}

bool IncomingPacketHandler::parseCreatureMoveNorth(Message& msg) {
	printf("[parseCreatureMoveNorth]\n");
	return true;
}

bool IncomingPacketHandler::parseCreatureMoveEast(Message& msg) {
	printf("[parseCreatureMoveEast]\n");
	return true;
}

bool IncomingPacketHandler::parseCreatureMoveSouth(Message& msg) {
	printf("[parseCreatureMoveSouth]\n");
	return true;
}

bool IncomingPacketHandler::parseCreatureMoveWest(Message& msg) {
	printf("[parseCreatureMoveWest]\n");
	return true;
}

bool IncomingPacketHandler::parseCreatureSpeak(Message& msg) {
	Position position;
	uint16_t channelId = 0;
	uint32_t time = 0;
	uint32_t unkSpeak = msg.getU32();
	std::string name = msg.getString();
	uint16_t level = msg.getU16();
	uint8_t type = msg.getByte();

	switch (type) {
	case SPEAK_SAY:
	case SPEAK_WHISPER:
	case SPEAK_YELL:
	case SPEAK_MONSTER_SAY:
	case SPEAK_MONSTER_YELL:
	case SPEAK_PRIVATE_NP:
		position = msg.getPosition();
		break;
	case SPEAK_CHANNEL_Y:
	case SPEAK_CHANNEL_W:
	case SPEAK_CHANNEL_R1:
	case SPEAK_CHANNEL_R2:
	case SPEAK_CHANNEL_O:
		channelId = msg.getU16();
		break;
	case SPEAK_RVR_CHANNEL:
		time = msg.getU32();
		break;
	default:
		break;
	}
	std::string message = msg.getString();

	std::cout << "[parseCreatureSpeak] " << name << ": " << message << std::endl;
	return true;
}

bool IncomingPacketHandler::parsePlayerStats(Message& msg) {
	uint16_t health = msg.getU16();
	uint16_t healthMax = msg.getU16();
	uint32_t capacity = msg.getU32();
	uint32_t experience = msg.getU32();
	uint64_t level = msg.getU64();
	uint8_t levelPercent = msg.getByte();
	uint16_t mana = msg.getU16();
	uint16_t manaMax = msg.getU16();
	uint8_t magicLevel = msg.getByte();
	uint8_t magicLevelPercent = msg.getByte();
	uint8_t soul = msg.getByte();
	uint16_t stamina = msg.getU16();

	std::cout << "[parsePlayerStats]" << std::endl;
	return true;
}

bool IncomingPacketHandler::parseAddSelf(Message& msg) {
	uint32_t id = msg.getU32();
	uint16_t drawSpeed = msg.getU16();
	uint8_t canReportBugs = msg.getByte();

	std::cout << "[parseAddSelf]" << std::endl;
	return true;
}

bool IncomingPacketHandler::parseMapDescription(Message& msg) {
	Position position = msg.getPosition();

	std::cout << "[parseMapDescription] new position: " << position << std::endl;
	return true;
}

bool IncomingPacketHandler::parseCreatureHealth(Message& msg) {
	uint32_t id = msg.getU32();
	uint8_t percent = msg.getByte();

	std::cout << "[parseCreatureHealth] ID: " << id << " has " << (int) percent << "%" << std::endl;
	return true;
}

bool IncomingPacketHandler::parseCreatureMove(Message& msg) {
	Position from = msg.getPosition();
	uint8_t fromStack = msg.getByte();
	Position to = msg.getPosition();

	std::cout << "[parseCreatureMove] to " << to << std::endl;
	return true;
}

bool IncomingPacketHandler::parsePing(Message& msg) {
	return true;
}

bool IncomingPacketHandler::parseTextMessage(Message& msg) {
	uint8_t type = msg.getByte();
	std::string message = msg.getString();
	std::cout << "[parseTextMessage] " << message << std::endl;
	return true;
}

bool OutgoingPacketHandler::parse(Message& msg) {
	unsigned char type = msg.getByte();
	switch (type) {
	case 0x14:
		return parseLogout(msg);
	case 0x1E:
		return parsePing(msg);
	case 0x96:
		return parseSay(msg);
	}
	return parseUnknownPacket(type, msg);
}

OutgoingPacketHandler::OutgoingPacketHandler(Hook* handler) :
		PacketHandler(handler) {
}

bool OutgoingPacketHandler::parseUnknownPacket(unsigned char type, Message& msg) {
	msg.setBufferPosition(msg.getBufferLength());
	return true;
}

bool OutgoingPacketHandler::parseSay(Message& msg) {
	uint8_t type = msg.getByte();
	uint16_t channelId = 0;
	std::string receiver;
	switch (type) {
	case SPEAK_PRIVATE:
	case SPEAK_PRIVATE_RED:
	case SPEAK_RVR_ANSWER:
		receiver = msg.getString();
		break;
	case SPEAK_CHANNEL_Y:
	case SPEAK_CHANNEL_R1:
	case SPEAK_CHANNEL_R2:
		channelId = msg.getU16();
		break;
	default:
		break;
	}
	std::string message = msg.getString();
	char command_code = '%';
	char args_seperator = ' ';
	if (message[0] == command_code) {
		std::string call = message.substr(1, message.length() - 1);
		std::string token;
		std::list < std::string > tokens;
		/* Split in one iteration*/
		for (std::string::iterator it = call.begin(); it != call.end(); it++) {
			if (*it == args_seperator) {
				tokens.push_back(token);
				token.erase();
				continue;
			}
			token.push_back(*it);
		}
		tokens.push_back(token);
		std::string name = tokens.front();
		tokens.pop_front();
		printf("[OutgoingPacketHandler::parseSay] executing call %s\n", name.c_str());
		return false;
	}
	/* Another example */
	if (message.find("fuck") != std::string::npos) {
		std::cout << "[parseSay] blocking text: " << message << std::endl;
		return false;
	}
	std::cout << "[parseSay] " << message << std::endl;
	return true;
}

bool OutgoingPacketHandler::parsePing(Message& msg) {
	return true;
}

bool OutgoingPacketHandler::parseLogout(Message& msg) {
	_hook->setLoggedIn(false);

	std::cout << "[parseLogout]" << std::endl;
	return true;
}
