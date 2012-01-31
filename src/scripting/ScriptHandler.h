#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include <QObject>
#include <QFile>
#include <QHash>
#include <QStringList>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptEngineAgent>

#include "Handler.h"
#include "ModuleManager.h"
#include "Hook.h"
#include "EncryptedMessage.h"

class ScriptHandler: public Handler, public ModuleManager {
	Q_OBJECT

public:
	ScriptHandler(Hook*);

	Hook* hook();
	QScriptEngine* engine();

	void reload();

	void receiveFromClient(const EncryptedMessage*);
	void receiveFromServer(const EncryptedMessage*);

private:
	bool receiveFromClientInternal(const EncryptedMessage*);

	Hook* hook_;

	QScriptEngine engine_;
	QScriptString receiveFromClientHandle_;
	QScriptString receiveFromServerHandle_;
};

#endif /* SCRIPTHANDLER_H_ */
