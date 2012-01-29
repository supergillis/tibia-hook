#include "EnvironmentModule.h"

EnvironmentModule::EnvironmentModule(QObject* parent) :
		Module(parent) {
}

void EnvironmentModule::install(ScriptHandler* handler) {
	ScriptEngine* engine = handler->getScriptEngine();
	QScriptValue reloadFunction = engine->newFunction(EnvironmentModule::reload);
	reloadFunction.setData(engine->newQObject(this));

	QScriptValue environmentObject = handler->createInstance(handler->getClassObject(), engine->newArray(0));
	environmentObject.setProperty("reload", reloadFunction);
	environmentObject.setProperty("require", engine->newFunction(EnvironmentModule::require));

	engine->globalObject().setProperty("Environment", environmentObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

QScriptValue EnvironmentModule::reload(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue value = context->callee().data();
		ScriptHandler* handler = qobject_cast<ScriptHandler*>(value.toQObject());
		if (handler) {
			handler->reload();
			return QScriptValue(true);
		}
	}
	return context->throwError("invalid call to reload()");
}

QScriptValue EnvironmentModule::require(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ScriptEngine* scriptEngine = qobject_cast<ScriptEngine*>(engine);
		if (value.isString() && scriptEngine) {
			QString required = value.toString();
			return scriptEngine->require(required);
		}
	}
	return context->throwError("invalid call to require(String)");
}
