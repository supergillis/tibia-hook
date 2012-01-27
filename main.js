print("evaluating script");

require("outgoing_packet.js");

Handler.out = OutgoingHandler.create();

Handler.handleOutgoingPacket = function(packet) {
	return Handler.out.handle(packet);
}

Handler.handleIncomingPacket = function(packet) {
}
