#ifndef ENVIRONMENTMODULE_H_
#define ENVIRONMENTMODULE_H_

#include <QFile>
#include <QObject>
#include <QStringList>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ModuleManager.h"

class EnvironmentModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const QString VARIABLE_NAME;

	EnvironmentModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

public slots:
	bool reload();
	bool require(const QString&);
	bool require(QFile& file);

private:
	QScriptEngine* engine_;
	QStringList requiredFiles_;
};

#endif /* ENVIRONMENTMODULE_H_ */
