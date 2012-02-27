#ifndef DEBUGGER_MODULE_H
#define DEBUGGER_MODULE_H

#include <QScriptEngine>
#include <QScriptEngineAgent>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class Debugger;
class DebuggerPlugin: public QObject, public ScriptPluginInterface {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	QString name() const;
	int version() const;

	void install(ScriptEngineInterface*);
	void uninstall();

private:
	Debugger* debugger_;
};

class Debugger: public QScriptEngineAgent {
public:
	Debugger(QScriptEngine*);

	void exceptionThrow(qint64, const QScriptValue&, bool);
};

#endif /* DEBUGGER_MODULE_H */
