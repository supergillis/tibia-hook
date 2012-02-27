#include "ClassPlugin.h"

EXPORT_PLUGIN(ClassPlugin)

const QString ClassPlugin::PLUGIN_NAME("class");
const int ClassPlugin::PLUGIN_VERSION(1);

const QString ClassPlugin::VARIABLE_NAME("Class");

QString ClassPlugin::name() const {
	return PLUGIN_NAME;
}

int ClassPlugin::version() const {
	return PLUGIN_VERSION;
}

void ClassPlugin::install(ScriptEngineInterface* engine) {
	engine_ = engine;

	instanceHandle_ = engine_->toStringHandle("instance");
	extendHandle_ = engine_->toStringHandle("extend");
	extendedHandle_ = engine_->toStringHandle("extended");
	createHandle_ = engine_->toStringHandle("create");
	constructorHandle_ = engine_->toStringHandle("constructor");

	rootClass_ = engine_->newObject();
	rootClass_.setData(engine_->newQObject(this));
	rootClass_.setPrototype(createClassPrototype());
	engine_->globalObject().setProperty(VARIABLE_NAME, rootClass_, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void ClassPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
}

QScriptValue ClassPlugin::createClassPrototype() {
	QScriptValue global = engine_->globalObject();
	QScriptValue prototype = engine_->newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty(extendHandle_, engine_->newFunction(ClassPlugin::extend), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(extendedHandle_, engine_->newFunction(ClassPlugin::extended), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(instanceHandle_, createInstancePrototype(), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(createHandle_, engine_->newFunction(ClassPlugin::create), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ClassPlugin::extendRootClassWithArgs(QScriptValue arguments) {
	return extendClassWithArgs(rootClass_, arguments);
}

QScriptValue ClassPlugin::extendRootClass() {
	return extendClass(rootClass_);
}

QScriptValue ClassPlugin::extendClassWithArgs(QScriptValue prototype, QScriptValue arguments) {
	QScriptValue instancePrototype = engine_->newObject();
	instancePrototype.setPrototype(prototype.property(instanceHandle_));
	QScriptValue result = engine_->newObject();
	result.setPrototype(prototype);
	result.setProperty(instanceHandle_, instancePrototype, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.property(extendedHandle_).call(result, arguments);
	return result;
}

QScriptValue ClassPlugin::extendClass(QScriptValue prototype) {
	return extendClassWithArgs(prototype, engine_->newArray(0));
}

QScriptValue ClassPlugin::createInstancePrototype() {
	QScriptValue global = engine_->globalObject();
	QScriptValue prototype = engine_->newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty(constructorHandle_, engine_->newFunction(ClassPlugin::constructor), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ClassPlugin::createRootInstanceWithArgs(QScriptValue arguments) {
	return createInstanceWithArgs(rootClass_, arguments);
}

QScriptValue ClassPlugin::createRootInstance() {
	return createInstance(rootClass_);
}

QScriptValue ClassPlugin::createInstanceWithArgs(QScriptValue parent, QScriptValue arguments) {
	QScriptValue prototype = parent.property(instanceHandle_);
	QScriptValue result = engine_->newObject();
	result.setPrototype(prototype);
	prototype.property(constructorHandle_).call(result, arguments);
	return result;
}

QScriptValue ClassPlugin::createInstance(QScriptValue parent) {
	return createInstanceWithArgs(parent, engine_->newArray(0));
}

QScriptValue ClassPlugin::extend(QScriptContext* context, QScriptEngine* engine) {
	QScriptValue rootClass = engine->globalObject().property(VARIABLE_NAME);
	ClassPlugin* module = qobject_cast<ClassPlugin*>(rootClass.data().toQObject());
	if (module) {
		QScriptValue parent = context->thisObject();
		return module->extendClassWithArgs(parent, context->argumentsObject());
	}
	return context->throwError("invalid call to extend()");
}

QScriptValue ClassPlugin::create(QScriptContext* context, QScriptEngine* engine) {
	QScriptValue rootClass = engine->globalObject().property(VARIABLE_NAME);
	ClassPlugin* module = qobject_cast<ClassPlugin*>(rootClass.data().toQObject());
	if (module) {
		QScriptValue parent = context->thisObject();
		return module->createInstanceWithArgs(parent, context->argumentsObject());
	}
	return context->throwError("invalid call to create()");
}

QScriptValue ClassPlugin::extended(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}

QScriptValue ClassPlugin::constructor(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}
