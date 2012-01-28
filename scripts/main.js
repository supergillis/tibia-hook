print("evaluating script");

Environment.require("class.js");
Environment.require("enum.js");
Environment.require("message.js");
Environment.require("outgoing/handler.js");
Environment.require("plugins/command.js");
Environment.require("plugins/turn.js");

Handler.handleOutgoingPacket = function(packet) {
	return OutgoingHandler.handle(packet);
}

Handler.handleIncomingPacket = function(packet) {
}
