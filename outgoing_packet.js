require("class.js");
require("packet_type.js");
require("enum.js");

OutgoingHandler = Class.extend();

(function() {
	PacketSpeak = PacketType.extend(OutgoingMessageType.Speak);

	PacketSpeak.instance.deserialize = function(packet) {
		this.type = packet.readU8();
		this.channel = 0;
		this.receiver = "";
		switch (this.type) {
			case SpeakType.PrivateTo:
			case SpeakType.GamemasterPrivateTo:
				this.receiver = packet.readString();
				break;
			case SpeakType.Channel:
			case SpeakType.ChannelManagement:
			case SpeakType.ChannelHighlight:
				this.channel = packet.readU16();
				break;
		}
		this.message = packet.readString();
	};

	PacketSpeak.instance.serialize = function(packet) {
		packet.writeU8(this.type);
		switch (this.type) {
			case SpeakType.PrivateTo:
			case SpeakType.GamemasterPrivateTo:
				packet.writeString(this.receiver);
				break;
			case SpeakType.Channel:
			case SpeakType.ChannelManagement:
			case SpeakType.ChannelHighlight:
				packet.writeU16(this.channel);
				break;
		}
		packet.writeString(this.message);
	};

	OutgoingHandler.instance.handle = function(packet) {
		type = packet.readU8();
		switch (type) {
			case OutgoingMessageType.Speak:
				return this.parseSpeak(packet);
				break;
		}
		return false;
	};

	OutgoingHandler.instance.parseSpeak = function(packet) {
		speak = PacketSpeak.deserialize(packet);
		if (speak.message == "lol") {
			hooked = new Packet();
			hooked.writeU8(PacketSpeak.type);
			double = PacketSpeak.create();
			double.type = speak.type;
			double.channel = speak.channel;
			double.receiver = speak.receiver;
			double.message = "lolol";
			double.serialize(hooked);
			Hook.write(hooked);
			return true;
		}
		return false;
	};
})();
