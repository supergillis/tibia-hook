#include "ClassModule.h"

const QString ClassModule::PLUGIN_NAME("class");
const QString ClassModule::VARIABLE_NAME("Class");

ClassModule::ClassModule(QObject* parent) :
		Module(parent) {
}

QString ClassModule::name() const {
	return PLUGIN_NAME;
}

bool ClassModule::install(ModuleManager* manager) {
	engine_ = manager->engine();

	instanceHandle_ = engine_->toStringHandle("instance");
	extendHandle_ = engine_->toStringHandle("extend");
	extendedHandle_ = engine_->toStringHandle("extended");
	createHandle_ = engine_->toStringHandle("create");
	constructorHandle_ = engine_->toStringHandle("constructor");

	rootClass_ = engine_->newObject();
	rootClass_.setData(engine_->newQObject(this));
	rootClass_.setPrototype(createClassPrototype());
	engine_->globalObject().setProperty(VARIABLE_NAME, rootClass_, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

QScriptValue ClassModule::createClassPrototype() {
	QScriptValue global = engine_->globalObject();
	QScriptValue prototype = engine_->newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty(extendHandle_, engine_->newFunction(ClassModule::extend), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(extendedHandle_, engine_->newFunction(ClassModule::extended), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(instanceHandle_, createInstancePrototype(), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(createHandle_, engine_->newFunction(ClassModule::create), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ClassModule::extendRootClassWithArgs(QScriptValue arguments) {
	return extendClassWithArgs(rootClass_, arguments);
}

QScriptValue ClassModule::extendRootClass() {
	return extendClass(rootClass_);
}

QScriptValue ClassModule::extendClassWithArgs(QScriptValue prototype, QScriptValue arguments) {
	QScriptValue instancePrototype = engine_->newObject();
	instancePrototype.setPrototype(prototype.property("instance"));
	QScriptValue result = engine_->newObject();
	result.setPrototype(prototype);
	result.setProperty(instanceHandle_, instancePrototype, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.property(extendedHandle_).call(result, arguments);
	return result;
}

QScriptValue ClassModule::extendClass(QScriptValue prototype) {
	return extendClassWithArgs(prototype, engine_->newArray(0));
}

QScriptValue ClassModule::createInstancePrototype() {
	QScriptValue global = engine_->globalObject();
	QScriptValue prototype = engine_->newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty(constructorHandle_, engine_->newFunction(ClassModule::constructor), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ClassModule::createRootInstanceWithArgs(QScriptValue arguments) {
	return createInstanceWithArgs(rootClass_, arguments);
}

QScriptValue ClassModule::createRootInstance() {
	return createInstance(rootClass_);
}

QScriptValue ClassModule::createInstanceWithArgs(QScriptValue parent, QScriptValue arguments) {
	QScriptValue prototype = parent.property(instanceHandle_);
	QScriptValue result = engine_->newObject();
	result.setPrototype(prototype);
	prototype.property(constructorHandle_).call(result, arguments);
	return result;
}

QScriptValue ClassModule::createInstance(QScriptValue parent) {
	return createInstanceWithArgs(parent, engine_->newArray(0));
}

QScriptValue ClassModule::extend(QScriptContext* context, QScriptEngine* engine) {
	QScriptValue rootClass = engine->globalObject().property(VARIABLE_NAME);
	ClassModule* module = qobject_cast<ClassModule*>(rootClass.data().toQObject());
	if (module) {
		QScriptValue parent = context->thisObject();
		return module->extendClassWithArgs(parent, context->argumentsObject());
	}
	return context->throwError("invalid call to extend()");
}

QScriptValue ClassModule::create(QScriptContext* context, QScriptEngine* engine) {
	QScriptValue rootClass = engine->globalObject().property(VARIABLE_NAME);
	ClassModule* module = qobject_cast<ClassModule*>(rootClass.data().toQObject());
	if (module) {
		QScriptValue parent = context->thisObject();
		return module->createInstanceWithArgs(parent, context->argumentsObject());
	}
	return context->throwError("invalid call to create()");
}

QScriptValue ClassModule::extended(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}

QScriptValue ClassModule::constructor(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}
