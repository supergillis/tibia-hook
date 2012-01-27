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

private:
	static QScriptValue require(QScriptContext*, QScriptEngine*);
	static QScriptValue packetConstructor(QScriptContext*, QScriptEngine*);
	static QScriptValue hookWrite(QScriptContext*, QScriptEngine*);

	ScriptEngine _engine;
	QScriptValue _handler;
};

#endif /* SCRIPTHANDLER_H_ */
