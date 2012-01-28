TurnPlugin = Class.extend();

TurnPlugin.DirectionNames = {};
TurnPlugin.DirectionNames[OutgoingMessageType.TurnNorth] = "north";
TurnPlugin.DirectionNames[OutgoingMessageType.TurnEast] = "east";
TurnPlugin.DirectionNames[OutgoingMessageType.TurnSouth] = "south";
TurnPlugin.DirectionNames[OutgoingMessageType.TurnWest] = "west";

TurnPlugin.parse = function(message) {
	print("turning " + TurnPlugin.DirectionNames[message.superclass.type]);
	return false;
};

OutgoingHandler.registerMessageCallback(MessageTurnNorth, TurnPlugin.parse);
OutgoingHandler.registerMessageCallback(MessageTurnEast, TurnPlugin.parse);
OutgoingHandler.registerMessageCallback(MessageTurnSouth, TurnPlugin.parse);
OutgoingHandler.registerMessageCallback(MessageTurnWest, TurnPlugin.parse);
