#ifndef ENVIRONMENTMODULE_H_
#define ENVIRONMENTMODULE_H_

#include <QFile>
#include <QObject>
#include <QStringList>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class EnvironmentModule: public QObject, public Module {
	Q_OBJECT

public:
	QString name() const;

	void install();

	static void setRequiredFiles(QScriptEngine*, const QStringList&);
	static QStringList requiredFiles(QScriptEngine*);

	static QScriptValue require(QScriptEngine*, const QString&);
	static QScriptValue require(QScriptEngine*, QFile& file);

private:
	static QScriptValue reload(QScriptContext*, QScriptEngine*);
	static QScriptValue require(QScriptContext*, QScriptEngine*);
};

#endif /* ENVIRONMENTMODULE_H_ */
