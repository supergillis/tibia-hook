CommandPlugin = Class.extend();

BattleList = Class.extend();
BattleList.Address = 0x844d360;
BattleList.Length = 250;

BattleListEntry = Class.extend();
BattleListEntry.Length = 172;
BattleListEntry.IdOffset = 0;
BattleListEntry.IdLength = 4;
BattleListEntry.NameOffset = 4;
BattleListEntry.NameLength = 32;
BattleListEntry.XOffset = 36;
BattleListEntry.YOffset = 40;
BattleListEntry.ZOffset = 44;
BattleListEntry.ScreenOffsetHOffset = 48;
BattleListEntry.ScreenOffsetVOffset = 52;
BattleListEntry.IsWalkingOffset = 76;
BattleListEntry.DirectionOffset = 80;
BattleListEntry.OutfitOffset = 96;
BattleListEntry.ColorHeadOffset = 100;
BattleListEntry.ColorBodyOffset = 104;
BattleListEntry.ColorLegsOffset = 108;
BattleListEntry.ColorFeetOffset = 112;
BattleListEntry.AddonOffset = 116;
BattleListEntry.MountIdOffset = 120;
BattleListEntry.LightOffset = 124;
BattleListEntry.LightColorOffset = 128;
BattleListEntry.BlackSquareOffset = 136;
BattleListEntry.HPBarOffset = 140;
BattleListEntry.WalkSpeedOffset = 144;
BattleListEntry.IsVisibleOffset = 148;
BattleListEntry.SkullOffset = 152;
BattleListEntry.PartyOffset = 156;
BattleListEntry.WarIconOffset = 164;
BattleListEntry.IsBlockingOffset = 168;

CommandPlugin.Commands = {};

CommandPlugin.Commands["lol"] = function(speak, arguments) {
	var packet = new Packet();
	packet.writeU8(MessageSpeak.type);
	double = MessageSpeak.create();
	double.type = speak.type;
	double.channel = speak.channel;
	double.receiver = speak.receiver;
	double.message = "lolol";
	double.serialize(packet);
	Network.sendToServer(packet);
};

CommandPlugin.Commands["battle"] = function(speak, arguments) {
	print("**** battle list ****");
	var index;
	for(index = 0; index < BattleList.Length - 1; index++) {
		var address = BattleList.Address + (index * BattleListEntry.Length);
		var name = Memory.readString(address + BattleListEntry.NameOffset);
		var visible = Memory.readU32(address + BattleListEntry.IsVisibleOffset);
		if(name.length == 0)
			break;
		print(name + " " + visible);
	}
};

CommandPlugin.Commands["fake"] = function(speak, arguments) {
	var packet = new Packet();
	packet.writeU8(MessageText.type);
	message = MessageText.create();
	message.type = 17;
	message.message = "test";
	message.serialize(packet);
	Network.sendToClient(packet);
}

CommandPlugin.Commands["reload"] = function(speak, arguments) {
	Environment.reload();
};

CommandPlugin.Commands["move"] = function(speak, arguments) {
	var direction = arguments.shift();
	var key;
	switch (direction) {
		case "up":
			key = Keys.XK_Up;
			break;
		case "right":
			key = Keys.XK_Right;
			break;
		case "down":
			key = Keys.XK_Down;
			break;
		case "left":
			key = Keys.XK_Left;
			break;
		default:
			key = Keys.XK_Up
	}
	Client.sendKeyPress(key);
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
