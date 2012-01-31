#ifndef CLIENTMODULE_H
#define CLIENTMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class ClientModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;

	ClientModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

private:
	static QScriptValue sendPacket(QScriptContext*, QScriptEngine*);
	static QScriptValue sendKeyPress(QScriptContext*, QScriptEngine*);

	static const QString VARIABLE_NAME;
};

#endif /* CLIENTMODULE_H */
