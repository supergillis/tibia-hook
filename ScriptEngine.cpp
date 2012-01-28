#include "ScriptEngine.h"

#include <QDebug>
#include <QDir>

ScriptEngine::ScriptEngine(QObject* parent) :
		QScriptEngine(parent), QScriptEngineAgent(this) {
	setAgent(this);
}

void ScriptEngine::clearRequiredFiles() {
	_required.clear();
}

QScriptValue ScriptEngine::require(const QString& path) {
	QDir scripts = QDir::current();
	if (!scripts.cd("scripts")) {
		qDebug() << "could not find scripts directory" << path;
		return QScriptValue(false);
	}

	if (_required.contains(path)) {
		qDebug() << "already required" << path;
		return QScriptValue(false);
	}

	QFile file(scripts.absoluteFilePath(path));
	if (!file.open(QFile::ReadOnly)) {
		qDebug() << "could not require" << path;
		return QScriptValue(false);
	}

	evaluate(file.readAll(), path);
	_required << path;

	return QScriptValue(true);
}

void ScriptEngine::exceptionThrow(qint64 scriptId, const QScriptValue& exception, bool hasHandler) {
	if (!hasHandler) {
		qWarning() << "unhandled exception";
		qWarning() << exception.toString().toAscii();
		qWarning() << uncaughtExceptionBacktrace();
	}
}
