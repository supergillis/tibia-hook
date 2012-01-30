#include "ClassModule.h"

QString ClassModule::name() const {
	return "class";
}

void ClassModule::install() {
	QScriptEngine* engine = handler()->scriptEngine();
	rootClass_ = engine->newObject();
	rootClass_.setPrototype(ClassModule::createClassPrototype(engine));
	engine->globalObject().setProperty("Class", rootClass_, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

QScriptValue ClassModule::extend(QScriptContext* context, QScriptEngine* engine) {
	QScriptValue parent = context->thisObject();
	return ClassModule::extendClassWithArgs(engine, parent, context->argumentsObject());
}

QScriptValue ClassModule::create(QScriptContext* context, QScriptEngine* engine) {
	QScriptValue parent = context->thisObject();
	return ClassModule::createInstanceWithArgs(engine, parent, context->argumentsObject());
}

QScriptValue ClassModule::extended(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}

QScriptValue ClassModule::constructor(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}

QScriptValue ClassModule::createClassPrototype(QScriptEngine* engine) {
	QScriptValue global = engine->globalObject();
	QScriptValue prototype = engine->newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty("extend", engine->newFunction(ClassModule::extend), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty("extended", engine->newFunction(ClassModule::extended), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty("instance", ClassModule::createInstancePrototype(engine), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty("create", engine->newFunction(ClassModule::create), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ClassModule::extendClassWithArgs(QScriptEngine* engine, QScriptValue prototype, QScriptValue arguments) {
	QScriptValue instancePrototype = engine->newObject();
	instancePrototype.setPrototype(prototype.property("instance"));
	QScriptValue result = engine->newObject();
	result.setPrototype(prototype);
	result.setProperty("instance", instancePrototype, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.property("extended").call(result, arguments);
	return result;
}

QScriptValue ClassModule::extendClass(QScriptEngine* engine, QScriptValue prototype) {
	return extendClassWithArgs(engine, prototype, engine->newArray(0));
}

QScriptValue ClassModule::createInstancePrototype(QScriptEngine* engine) {
	QScriptValue global = engine->globalObject();
	QScriptValue prototype = engine->newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty("constructor", engine->newFunction(ClassModule::constructor), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ClassModule::createInstanceWithArgs(QScriptEngine* engine, QScriptValue parent, QScriptValue arguments) {
	QScriptValue prototype = parent.property("instance");
	QScriptValue result = engine->newObject();
	result.setPrototype(prototype);
	prototype.property("constructor").call(result, arguments);
	return result;
}

QScriptValue ClassModule::createInstance(QScriptEngine* engine, QScriptValue parent) {
	return createInstanceWithArgs(engine, parent, engine->newArray(0));
}
