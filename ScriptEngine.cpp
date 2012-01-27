#include "ScriptEngine.h"

#include <QDebug>

ScriptEngine::ScriptEngine(QObject* parent) :
		QScriptEngine(parent), QScriptEngineAgent(this) {
	setAgent(this);
}

QScriptValue ScriptEngine::require(const QString& path) {
	if (required.contains(path)) {
		qDebug() << "already required" << path;
		return QScriptValue(false);
	}

	QFile file(path);
	if (!file.open(QFile::ReadOnly)) {
		qDebug() << "could not require" << path;
		return QScriptValue(false);
	}

	evaluate(file.readAll());
	required << path;

	return QScriptValue(true);
}

void ScriptEngine::exceptionThrow(qint64 scriptId, const QScriptValue& exception, bool hasHandler) {
	if (!hasHandler) {
		qWarning() << "unhandled exception";
		qWarning() << exception.toString().toAscii();
	}
}
