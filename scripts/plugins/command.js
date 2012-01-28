CommandPlugin = Class.extend();

CommandPlugin.Commands = {};

CommandPlugin.Commands["lol"] = function(speak, arguments) {
	hooked = new Packet();
	hooked.writeU8(MessageSpeak.type);
	double = MessageSpeak.create();
	double.type = speak.type;
	double.channel = speak.channel;
	double.receiver = speak.receiver;
	double.message = "lolol";
	double.serialize(hooked);
	Hook.write(hooked);
};

CommandPlugin.Commands["reload"] = function(speak, arguments) {
	Environment.reload();
};

CommandPlugin.parse = function(speak) {
	if (speak.message.charAt() != "%")
		return false;

	var arguments = speak.message.substr(1).split(" ");
	var command = arguments.shift();
	var callback = CommandPlugin.Commands[command];
	if (callback != undefined)
		callback(speak, arguments);

	return true;
};

OutgoingHandler.registerMessageCallback(MessageSpeak, CommandPlugin.parse);
