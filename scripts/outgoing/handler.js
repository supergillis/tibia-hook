OutgoingHandler = Class.extend();

(function() {
	OutgoingHandler.messageCallbacks = {};
	OutgoingHandler.messageClasses = {};

	OutgoingHandler.registerMessageCallback = function(messageClass, callback) {
		var callbacks = [];
		if (this.messageCallbacks[messageClass.type] != undefined) {
			callbacks = this.messageCallbacks[messageClass.type];
		}
		callbacks.push(callback);
		this.messageCallbacks[messageClass.type] = callbacks;
	};

	OutgoingHandler.registerMessage = function(messageClass) {
		this.messageClasses[messageClass.type] = messageClass;
	};

	OutgoingHandler.handle = function(packet) {
		var type = packet.readU8();
		var callbacks = this.messageCallbacks[type];
		if (callbacks) {
			var messageClass = this.messageClasses[type];
			if (messageClass) {
				var message = messageClass.deserialize(packet);
				var index;
				for (index in callbacks) {
					var callback = callbacks[index];
					var result = callback(message);
					if (result)
						return true;
				}
			}
		}
		return false;
	};
})();

Environment.require("outgoing/messages/speak.js");
Environment.require("outgoing/messages/turn.js");
