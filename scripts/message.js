Environment.require("class.js");

Message = Class.extend();

Message.extended = function(type) {
	this.type = type;
}

Message.deserialize = function(packet) {
	message = this.create();
	message.deserialize(packet);
	return message;
};

Message.instance.deserialize = function(packet) {
};

Message.instance.serialize = function(packet) {
};
