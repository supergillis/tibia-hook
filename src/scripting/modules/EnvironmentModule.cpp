#include <QDir>

#include "EnvironmentModule.h"

const QString EnvironmentModule::PLUGIN_NAME("environment");
const QString EnvironmentModule::VARIABLE_NAME("Environment");

EnvironmentModule::EnvironmentModule(QObject* parent) :
		Module(parent), engine_(NULL) {
}

QString EnvironmentModule::name() const {
	return PLUGIN_NAME;
}

bool EnvironmentModule::install(ModuleManager* manager) {
	engine_ = manager->engine();
	engine_->globalObject().setProperty(VARIABLE_NAME, engine_->newQObject(this), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

bool EnvironmentModule::reload() {
	engine_->pushContext();
	requiredFiles_.clear();
	require("Main.js");
	engine_->popContext();
	return true;
}

bool EnvironmentModule::require(const QString& path) {
	QDir scripts(QDir::current());
	if (scripts.cd("scripts")) {
		QFile file(scripts.absoluteFilePath(path));
		return require(file);
	}
	return false;
}

bool EnvironmentModule::require(QFile& file) {
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
