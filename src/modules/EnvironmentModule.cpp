#include "EnvironmentModule.h"

EnvironmentModule::EnvironmentModule(QObject* parent) :
		Module(parent) {
}

void EnvironmentModule::install(ScriptHandler* handler) {
	QScriptEngine* engine = handler->scriptEngine();
	QScriptValue reloadFunction = engine->newFunction(EnvironmentModule::reload);
	reloadFunction.setData(engine->newQObject(this));

	QScriptValue environmentObject = handler->createInstance(handler->classObject(), engine->newArray(0));
	environmentObject.setProperty("reload", reloadFunction);
	environmentObject.setProperty("require", engine->newFunction(EnvironmentModule::require));

	engine->globalObject().setProperty("Environment", environmentObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

QScriptValue EnvironmentModule::reload(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		ScriptHandler* handler = qobject_cast<ScriptHandler*>(context->callee().data().toQObject());
		if (handler) {
			handler->reload();
			return true;
		}
	}
	return context->throwError("invalid call to reload()");
}

QScriptValue EnvironmentModule::require(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if (value.isString()) {
			ScriptHandler* handler = qobject_cast<ScriptHandler*>(context->callee().data().toQObject());
			if (handler) {
				QString required = value.toString();
				handler->require(required);
				return true;
			}
		}
	}
	return context->throwError("invalid call to require(String)");
}
