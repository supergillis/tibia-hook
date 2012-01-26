print("evaluating script");

SPEAK_SAY = 0x01, // normal talk
SPEAK_WHISPER = 0x02, // whispering - #w text
SPEAK_YELL = 0x03, // yelling - #y text
SPEAK_PRIVATE_PN = 0x04, // Player-to-NPC speaking(NPCs channel)
SPEAK_PRIVATE_NP = 0x05, // NPC-to-Player speaking
SPEAK_PRIVATE = 0x06, // Players speaking privately to players
SPEAK_CHANNEL_Y = 0x07, // Yellow message in chat
SPEAK_CHANNEL_W = 0x08, // White message in chat
SPEAK_RVR_CHANNEL = 0x09, // Reporting rule violation - Ctrl+R
SPEAK_RVR_ANSWER = 0x0A, // Answering report
SPEAK_RVR_CONTINUE = 0x0B, // Answering the answer of the report
SPEAK_BROADCAST = 0x0C, // Broadcast a message - #b
SPEAK_CHANNEL_R1 = 0x0D, // Talk red on chat - #c
SPEAK_PRIVATE_RED = 0x0E, // Red private - @name@ text
SPEAK_CHANNEL_O = 0x0F, // Talk orange on text
SPEAK_CHANNEL_R2 = 0x11, // Talk red anonymously on chat - #d
SPEAK_MONSTER_SAY = 0x13, // Talk orange
SPEAK_MONSTER_YELL = 0x14

packet = new Packet();
packet.writeU8(0x05);
packet.writeString("test");

packet.setPosition(0);
print("js " + packet.readU8());
print("js " + packet.readString());
print("js " + packet.readU8());
print("js " + packet.readU8());

OutgoingPacketHandler = {}

OutgoingPacketHandler.parseSay = function(packet) {
	type = packet.readU8();
	channel = 0;
	receiver = "";
	switch (type) {
		case SPEAK_PRIVATE:
		case SPEAK_PRIVATE_RED:
		case SPEAK_RVR_ANSWER:
			receiver = packet.readString();
			break;
		case SPEAK_CHANNEL_Y:
		case SPEAK_CHANNEL_R1:
		case SPEAK_CHANNEL_R2:
			channel = packet.readU16();
			break;
		default:
			break;
	}
	message = packet.readString();
	print("parseSay: " + message);
}

Handler.handleOutgoingPacket = function(packet) {
	type = packet.readU8();
	print("handleOutgoingPacket: 0x" + type.toString(16));

	switch (type) {
		case 0x96:
			OutgoingPacketHandler.parseSay(packet);
			break;
		default:
			return false;
	}
	return true;
}

Handler.handleIncomingPacket = function(packet) {
}
