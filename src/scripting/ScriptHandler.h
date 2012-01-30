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
#include "EncryptedMessage.h"
#include "Module.h"

class ScriptHandler: public Handler {
	Q_OBJECT

public:
	ScriptHandler(QObject* = 0);

	QScriptEngine* scriptEngine();
	const QScriptEngine* scriptEngine() const;

	bool install(Module*);

	Module* lookup(const QString& name);

	void reload();

	void receiveFromClient(const EncryptedMessage*);
	void receiveFromServer(const EncryptedMessage*);

private:
	bool receiveFromClientInternal(const EncryptedMessage*);

	QScriptEngine engine_;
	QScriptString receiveFromClientHandle_;
	QScriptString receiveFromServerHandle_;

	QHash<QString, Module*> modules_;
};

#endif /* SCRIPTHANDLER_H_ */
