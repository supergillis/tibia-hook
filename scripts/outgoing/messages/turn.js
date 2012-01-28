MessageTurnNorth = Message.extend(OutgoingMessageType.TurnNorth);
MessageTurnEast = Message.extend(OutgoingMessageType.TurnEast);
MessageTurnWest = Message.extend(OutgoingMessageType.TurnWest);
MessageTurnSouth = Message.extend(OutgoingMessageType.TurnSouth);

OutgoingHandler.registerMessage(MessageTurnNorth);
OutgoingHandler.registerMessage(MessageTurnEast);
OutgoingHandler.registerMessage(MessageTurnWest);
OutgoingHandler.registerMessage(MessageTurnSouth);
