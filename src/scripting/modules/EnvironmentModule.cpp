#include <QDir>

#include "EnvironmentModule.h"
#include "ClassModule.h"

const QString EnvironmentModule::PLUGIN_NAME("environment");
const QString EnvironmentModule::VARIABLE_NAME("Environment");

EnvironmentModule::EnvironmentModule(QObject* parent) :
		Module(parent) {
}

QString EnvironmentModule::name() const {
	return PLUGIN_NAME;
}

bool EnvironmentModule::install(ModuleManager* manager) {
	ClassModule* classModule = (ClassModule*) manager->lookup(ClassModule::PLUGIN_NAME);
	if (classModule) {
		engine_ = manager->engine();

		QScriptValue require = engine_->newFunction(EnvironmentModule::require);
		QScriptValue reload = engine_->newFunction(EnvironmentModule::reload);

		QScriptValue environmentObject = classModule->createRootInstance();
		environmentObject.setData(engine_->newQObject(this));
		environmentObject.setProperty("reload", reload);
		environmentObject.setProperty("require", require);
		engine_->globalObject().setProperty(VARIABLE_NAME, environmentObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		return true;
	}
	return false;
}

QScriptValue EnvironmentModule::reload() {
	engine_->pushContext();
	requiredFiles_.clear();
	require("Main.js");
	engine_->popContext();
	return true;
}

QScriptValue EnvironmentModule::require(const QString& path) {
	QDir scripts(QDir::current());
	if (scripts.cd("scripts")) {
		QFile file(scripts.absoluteFilePath(path));
		return require(file);
	}
	return false;
}

QScriptValue EnvironmentModule::require(QFile& file) {
	QString fileName = file.fileName();
	if (!requiredFiles_.contains(fileName)) {
		if (file.open(QFile::ReadOnly)) {
			engine_->evaluate(file.readAll(), fileName);
			requiredFiles_ << fileName;
			return true;
		}
	}
	return false;
}

QScriptValue EnvironmentModule::reload(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		EnvironmentModule* module = qobject_cast<EnvironmentModule*>(context->thisObject().data().toQObject());
		if (module) {
			return module->reload();
		}
	}
	return context->throwError("invalid call to reload()");
}

QScriptValue EnvironmentModule::require(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if (value.isString()) {
			QString required = value.toString();
			EnvironmentModule* module = qobject_cast<EnvironmentModule*>(context->thisObject().data().toQObject());
			if (module) {
				return module->require(required);
			}
		}
	}
	return context->throwError("invalid call to require(String)");
}
