#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include "Handler.h"
#include "Hook.h"
#include "EncryptedMessage.h"

#include <QScriptEngine>
#include <QScriptEngineAgent>
#include <QScriptContext>

class ScriptEngineAgent: public QScriptEngineAgent {
public:
	ScriptEngineAgent(QScriptEngine*);

	void exceptionThrow(qint64, const QScriptValue&, bool);
};

class ScriptHandler: public Handler {
	Q_OBJECT

public:
	ScriptHandler(Hook*);

	void handleOutgoingMessage(const EncryptedMessage&);
	bool handleOutgoingMessageInternal(const EncryptedMessage&);

	void handleIncomingMessage(const EncryptedMessage&);
	bool handleIncomingMessageInternal(const EncryptedMessage&);

private:
	static QScriptValue packetConstructor(QScriptContext*, QScriptEngine*);

	QScriptEngine _engine;
	ScriptEngineAgent _engineAgent;
	QScriptValue _handler;
};

#endif /* SCRIPTHANDLER_H_ */
