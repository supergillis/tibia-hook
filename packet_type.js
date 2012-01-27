require("class.js");

PacketType = Class.extend();

PacketType.extended = function(type) {
	this.type = type;
}

PacketType.deserialize = function(packet) {
	packetType = this.create();
	packetType.deserialize(packet);
	return packetType;
};
