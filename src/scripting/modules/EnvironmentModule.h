#ifndef ENVIRONMENTMODULE_H_
#define ENVIRONMENTMODULE_H_

#include <QFile>
#include <QObject>
#include <QStringList>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class EnvironmentModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;

	EnvironmentModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

	QScriptValue reload();
	QScriptValue require(const QString&);
	QScriptValue require(QFile& file);

private:
	QScriptEngine* engine_;
	QStringList requiredFiles_;

	static QScriptValue reload(QScriptContext*, QScriptEngine*);
	static QScriptValue require(QScriptContext*, QScriptEngine*);

	static const QString VARIABLE_NAME;
};

#endif /* ENVIRONMENTMODULE_H_ */
