#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include "Handler.h"
#include "Hook.h"

#include <QScriptEngine>

class ScriptHandler: public Handler {
	Q_OBJECT

public:
	ScriptHandler(Hook*);

	void handleOutgoingMessage(const EncryptedMessage&);
	bool handleOutgoingMessageInternal(const EncryptedMessage&);

	void handleIncomingMessage(const EncryptedMessage&);
	bool handleIncomingMessageInternal(const EncryptedMessage&);

private:
	QScriptEngine _engine;
	QScriptValue _handler;
};

#endif /* SCRIPTHANDLER_H_ */
