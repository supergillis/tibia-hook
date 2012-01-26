print("evaluating script");

Handler.handleOutgoingPacket = function(packet) {
	print("outgoing");
	print(packet.getName());
}

Handler.handleIncomingPacket = function(packet) {
}
