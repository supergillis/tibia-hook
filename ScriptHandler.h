#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include "Handler.h"
#include "EncryptedMessage.h"
#include "ScriptEngine.h"

#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptClass>

class ScriptHandler: public Handler {
	Q_OBJECT

public:
	ScriptHandler();

	void receiveFromClient(const EncryptedMessage*);
	bool receiveFromClientInternal(const EncryptedMessage*);

	void receiveFromServer(const EncryptedMessage*);
	bool receiveFromServerInternal(const EncryptedMessage*);

	void reload();

private:
	ScriptEngine _engine;
	QScriptValue _networkObject;
};

namespace Handlers {
	namespace Environment {
		static QScriptValue reload(QScriptContext*, QScriptEngine*);
		static QScriptValue require(QScriptContext*, QScriptEngine*);
	};

	namespace Network {
		static QScriptValue sendToServer(QScriptContext*, QScriptEngine*);
		static QScriptValue sendToClient(QScriptContext*, QScriptEngine*);
	};

	namespace Client {
		static QScriptValue sendPacket(QScriptContext*, QScriptEngine*);
		static QScriptValue sendKeyPress(QScriptContext*, QScriptEngine*);
	};

	namespace Packet {
		static QScriptValue constructor(QScriptContext*, QScriptEngine*);
	};

	namespace Memory {
		static QScriptValue readU8(QScriptContext*, QScriptEngine*);
		static QScriptValue readU16(QScriptContext*, QScriptEngine*);
		static QScriptValue readU32(QScriptContext*, QScriptEngine*);
		static QScriptValue readString(QScriptContext*, QScriptEngine*);
	};
};

#endif /* SCRIPTHANDLER_H_ */
