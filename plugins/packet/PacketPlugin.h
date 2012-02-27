#ifndef PACKETMODULE_H
#define PACKETMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class PacketPlugin: public QObject, public ScriptPluginInterface {
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
	static QScriptValue constructor(QScriptContext*, QScriptEngine*);

	ScriptEngineInterface* engine_;
};

#endif /* PACKETMODULE_H */
