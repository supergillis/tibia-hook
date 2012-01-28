CommandPlugin = Class.extend();

CommandPlugin.parse = function(message) {
	if (message.message == "lol") {
		hooked = new Packet();
		hooked.writeU8(MessageSpeak.type);
		double = MessageSpeak.create();
		double.type = message.type;
		double.channel = message.channel;
		double.receiver = message.receiver;
		double.message = "lolol";
		double.serialize(hooked);
		Hook.write(hooked);
		return true;
	}
	else if (message.message == "reload") {
		Environment.reload();
		return true;
	}
	return false;
};

OutgoingHandler.registerMessageCallback(MessageSpeak, CommandPlugin.parse);
