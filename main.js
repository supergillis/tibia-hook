print("evaluating script");

Handler.handleOutgoingPacket = function(packet) {
	type = packet.readU8();
	print("outgoing");
	print("type " + type.toString(16));
}

Handler.handleIncomingPacket = function(packet) {
}
