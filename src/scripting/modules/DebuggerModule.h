#ifndef DEBUGGER_MODULE_H
#define DEBUGGER_MODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class Debugger;
class DebuggerModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;

	DebuggerModule(QObject*);
	~DebuggerModule();

	QString name() const;

	bool install(ModuleManager*);

private:
	Debugger* debugger_;
};

class Debugger: public QScriptEngineAgent {
public:
	Debugger(QScriptEngine*);

	void exceptionThrow(qint64, const QScriptValue&, bool);
};

#endif /* DEBUGGER_MODULE_H */
