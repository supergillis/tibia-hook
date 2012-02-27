#ifndef ENVIRONMENTPLUGIN_H
#define ENVIRONMENTPLUGIN_H

#include <QFile>
#include <QObject>
#include <QStringList>
#include <QScriptEngine>
#include <QScriptContext>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class EnvironmentPlugin: public QObject, public ScriptPluginInterface {
Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	static const QString VARIABLE_NAME;

	QString name() const;
	int version() const;

	void install(ScriptEngineInterface*);
	void uninstall();

public slots:
	bool reload();
	bool require(const QString&);

private:
	ScriptEngineInterface* engine_;
};

#endif /* ENVIRONMENTPLUGIN_H */
