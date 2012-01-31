#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class NetworkModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;

	NetworkModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

private:
	static QScriptValue sendToServer(QScriptContext*, QScriptEngine*);
	static QScriptValue sendToClient(QScriptContext*, QScriptEngine*);

	static const QString VARIABLE_NAME;
};

#endif /* NETWORKMODULE_H */
