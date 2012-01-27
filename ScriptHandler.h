#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include "Handler.h"
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
	ScriptHandler();

	void handleOutgoingMessage(const EncryptedMessage&);
	bool handleOutgoingMessageInternal(const EncryptedMessage&);

	void handleIncomingMessage(const EncryptedMessage&);
	bool handleIncomingMessageInternal(const EncryptedMessage&);

private:
	static QScriptValue packetConstructor(QScriptContext*, QScriptEngine*);
	static QScriptValue hookWrite(QScriptContext*, QScriptEngine*);

	QScriptEngine _engine;
	ScriptEngineAgent _engineAgent;
	QScriptValue _handler;
	QScriptValue _hook;
};

#endif /* SCRIPTHANDLER_H_ */
