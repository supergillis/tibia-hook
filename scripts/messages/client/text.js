MessageText = Message.extend(0xB4);

MessageText.instance.deserialize = function(packet) {
	this.type = packet.readU8();
	this.message = packet.readString();
};

MessageText.instance.serialize = function(packet) {
	packet.writeU8(this.type);
	packet.writeString(this.message);
};
