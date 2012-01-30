#ifndef CLASSMODULE_H
#define CLASSMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class ClassModule: public Module {
public:
	QString name() const;

	void install();

	static QScriptValue extendClassWithArgs(QScriptEngine*, QScriptValue, QScriptValue);
	static QScriptValue extendClass(QScriptEngine*, QScriptValue);

	static QScriptValue createInstanceWithArgs(QScriptEngine*, QScriptValue, QScriptValue);
	static QScriptValue createInstance(QScriptEngine*, QScriptValue);

private:
	QScriptValue rootClass_;

	static QScriptValue createClassPrototype(QScriptEngine*);
	static QScriptValue createInstancePrototype(QScriptEngine*);

	static QScriptValue extend(QScriptContext*, QScriptEngine*);
	static QScriptValue extended(QScriptContext*, QScriptEngine*);
	static QScriptValue create(QScriptContext*, QScriptEngine*);
	static QScriptValue constructor(QScriptContext*, QScriptEngine*);
};

#endif /* CLASSMODULE_H */
