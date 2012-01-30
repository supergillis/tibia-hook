#ifndef ENVIRONMENTMODULE_H_
#define ENVIRONMENTMODULE_H_

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class EnvironmentModule: public Module {
	Q_OBJECT

public:
	EnvironmentModule(QObject*);

	void install(ScriptHandler*);

private:
	static QScriptValue reload(QScriptContext*, QScriptEngine*);
	static QScriptValue require(QScriptContext*, QScriptEngine*);

};

#endif /* ENVIRONMENTMODULE_H_ */
