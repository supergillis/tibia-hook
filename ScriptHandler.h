#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include "Handler.h"
#include "EncryptedMessage.h"
#include "ScriptEngine.h"

#include <QScriptEngine>
#include <QScriptContext>

class ScriptHandler: public Handler {
	Q_OBJECT

public:
	ScriptHandler();

	void handleOutgoingMessage(const EncryptedMessage*);
	bool handleOutgoingMessageInternal(const EncryptedMessage*);

	void handleIncomingMessage(const EncryptedMessage*);
	bool handleIncomingMessageInternal(const EncryptedMessage*);

	void reload();

private:
	ScriptEngine _engine;
	QScriptValue _handlerObject;
};

namespace Handlers {
	namespace Environment {
		static QScriptValue reload(QScriptContext*, QScriptEngine*);
		static QScriptValue require(QScriptContext*, QScriptEngine*);
	};

	namespace Packet {
		static QScriptValue constructor(QScriptContext*, QScriptEngine*);
	};

	namespace Hook {
		static QScriptValue write(QScriptContext*, QScriptEngine*);
	};

	namespace Memory {
		static QScriptValue readU8(QScriptContext*, QScriptEngine*);
		static QScriptValue readU16(QScriptContext*, QScriptEngine*);
		static QScriptValue readU32(QScriptContext*, QScriptEngine*);
		static QScriptValue readString(QScriptContext*, QScriptEngine*);
	};
};

#endif /* SCRIPTHANDLER_H_ */
