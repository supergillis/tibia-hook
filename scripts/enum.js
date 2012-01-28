OutgoingMessageType = {
	Speak: 0x96,
	TurnNorth: 0x6F,
	TurnEast: 0x70,
	TurnSouth: 0x71,
	TurnWest: 0x72
};

SpeakType = {
	None: 0x00,
	SpeakSay: 0x01,
	SpeakWhisper: 0x02,
	SpeakMonsterSay: 0x22,
	SpeakMonsterYell: 0x23,
	SpeakYell: 0x03,
	SpeakSpell: 0x09,
	PrivateFrom: 0x04,
	PrivateTo: 0x05,
	Channel: 0x07,
	ChannelManagement: 0x06,
	ChannelHighlight: 0x08,
	ChannelGuild: 0x1F, // guild messages
	NpcFrom: 0x0A,
	NpcTo: 0x0B,
	NpcTrade: 0x1E,
	GamemasterBroadcast: 0x0C,
	GamemasterChannel: 0x0D,
	GamemasterPrivateFrom: 0x0E,
	GamemasterPrivateTo: 0x0F,
	StatusConsoleRed: 0x0C, // red in console
	StatusConsoleOrange: 0x23, // orange message in the console
	StatusConsoleBlue: 0xFF,
	StatusDefault: 0x10, // white at bottom of game window & console
	StatusWarning: 0x11, // red in game window & console
	StatusSmall: 0x13, // white at bottom of game window
	EventAdvance: 0x12, // white in game window & console
	EventOrange: 0x22, // orange message in the console
	EventDefault: 0x1C, // white at bottom of game window & console
	InfoDescription: 0x14, // green in game window & console
	DamageDealt: 0x15,
	DamageReceiver: 0x16,
	DamageOthers: 0x19,
	Healed: 0x17,
	HealedOthers: 0x1A,
	Experience: 0x18,
	ExperienceOthers: 0x1B,
	Party: 0x21,
	PartyManagement: 0x20,
	Loot: 0x1D,
	Report: 0x24,
	HotkeyUse: 0x25,
	TutorialHint: 0x26
}
