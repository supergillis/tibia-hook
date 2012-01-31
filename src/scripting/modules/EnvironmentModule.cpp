#include <QDir>

#include "EnvironmentModule.h"
#include "ClassModule.h"

QString EnvironmentModule::name() const {
	return "environment";
}

void EnvironmentModule::install() {
	QScriptEngine* engine = handler()->engine();
	QScriptValue rootClass = engine->globalObject().property("Class");
	if (rootClass.isObject()) {
		QScriptValue require = engine->newFunction(EnvironmentModule::require);
		QScriptValue reload = engine->newFunction(EnvironmentModule::reload);

		QScriptValue environmentObject = ClassModule::createInstance(engine, rootClass);
		environmentObject.setProperty("reload", reload);
		environmentObject.setProperty("require", require);

		engine->globalObject().setProperty("Environment", environmentObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		EnvironmentModule::setRequiredFiles(engine, QStringList());
	}
	else {
		qDebug() << "could not find root class";
	}
}

void EnvironmentModule::setRequiredFiles(QScriptEngine* engine, const QStringList& list) {
	engine->globalObject().property("Environment").setData(engine->toScriptValue(list));
}

QStringList EnvironmentModule::requiredFiles(QScriptEngine* engine) {
	QScriptValue data = engine->globalObject().property("Environment").data();
	return engine->fromScriptValue<QStringList>(data);
}

QScriptValue EnvironmentModule::reload(QScriptEngine* engine) {
	engine->pushContext();
	EnvironmentModule::setRequiredFiles(engine, QStringList());
	EnvironmentModule::require(engine, "Main.js");
	engine->popContext();
	return true;
}

QScriptValue EnvironmentModule::require(QScriptEngine* engine, const QString& path) {
	QDir scripts(QDir::current());
	if (scripts.cd("scripts")) {
		QFile file(scripts.absoluteFilePath(path));
		return EnvironmentModule::require(engine, file);
	}
	return engine->currentContext()->throwError(scripts.absolutePath() + " does not exist");
}

QScriptValue EnvironmentModule::require(QScriptEngine* engine, QFile& file) {
	QStringList requiredFiles = EnvironmentModule::requiredFiles(engine);
	QString fileName = file.fileName();
	if (!requiredFiles.contains(fileName)) {
		if (file.open(QFile::ReadOnly)) {
			engine->evaluate(file.readAll(), fileName);
			requiredFiles << fileName;
			EnvironmentModule::setRequiredFiles(engine, requiredFiles);
			return true;
		}
		return engine->currentContext()->throwError(fileName + " does not exist");
	}
	return engine->currentContext()->throwError(fileName + " is already included");
}

QScriptValue EnvironmentModule::reload(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		return EnvironmentModule::reload(engine);
	}
	return context->throwError("invalid call to reload()");
}

QScriptValue EnvironmentModule::require(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if (value.isString()) {
			QString required = value.toString();
			EnvironmentModule::require(engine, required);
			return true;
		}
	}
	return context->throwError("invalid call to require(String)");
}
