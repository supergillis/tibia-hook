#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class NetworkPlugin: public QObject, public ScriptPluginInterface {
Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	static const QString VARIABLE_NAME;

	QString name() const;
	int version() const;

	void install(ScriptEngineInterface*);
	void uninstall();

private:
	ScriptEngineInterface* engine_;

	static QScriptValue sendToClient(QScriptContext*, QScriptEngine*);
	static QScriptValue sendToServer(QScriptContext*, QScriptEngine*);
};

#endif /* NETWORKMODULE_H */
