MessageSpeak = Message.extend(OutgoingMessageType.Speak);

MessageSpeak.instance.deserialize = function(packet) {
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

MessageSpeak.instance.serialize = function(packet) {
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

OutgoingHandler.registerMessage(MessageSpeak);
