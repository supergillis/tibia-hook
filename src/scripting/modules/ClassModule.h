#ifndef CLASSMODULE_H
#define CLASSMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class ClassModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;

	ClassModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

	QScriptValue extendRootClassWithArgs(QScriptValue);
	QScriptValue extendRootClass();
	QScriptValue extendClassWithArgs(QScriptValue, QScriptValue);
	QScriptValue extendClass(QScriptValue);

	QScriptValue createRootInstanceWithArgs(QScriptValue);
	QScriptValue createRootInstance();
	QScriptValue createInstanceWithArgs(QScriptValue, QScriptValue);
	QScriptValue createInstance(QScriptValue);

private:
	QScriptValue createClassPrototype();
	QScriptValue createInstancePrototype();

	QScriptEngine* engine_;
	QScriptValue rootClass_;

	QScriptString instanceHandle_;
	QScriptString extendHandle_;
	QScriptString extendedHandle_;
	QScriptString createHandle_;
	QScriptString constructorHandle_;

	static QScriptValue extend(QScriptContext*, QScriptEngine*);
	static QScriptValue extended(QScriptContext*, QScriptEngine*);
	static QScriptValue create(QScriptContext*, QScriptEngine*);
	static QScriptValue constructor(QScriptContext*, QScriptEngine*);

	static const QString VARIABLE_NAME;
};

#endif /* CLASSMODULE_H */
