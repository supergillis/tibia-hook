print("evaluating script");

Environment.require("class.js");
Environment.require("enum.js");
Environment.require("message.js");
Environment.require("messages/client/text.js");
Environment.require("outgoing/handler.js");
Environment.require("plugins/command.js");
//Environment.require("plugins/turn.js");

Network.receiveFromClient = function(packet) {
	return OutgoingHandler.handle(packet);
}

Network.receiveFromServer = function(packet) {
}
