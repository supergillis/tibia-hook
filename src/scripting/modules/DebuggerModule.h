#ifndef DEBUGGER_MODULE_H
#define DEBUGGER_MODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class Debugger;
class DebuggerModule: public Module {
public:
	DebuggerModule();
	~DebuggerModule();

	QString name() const;

	void install();

private:
	Debugger* debugger_;
};

class Debugger: public QScriptEngineAgent {
public:
	Debugger(QScriptEngine*);

	void exceptionThrow(qint64, const QScriptValue&, bool);
};

#endif /* DEBUGGER_MODULE_H */
