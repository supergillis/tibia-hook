#include "DebuggerPlugin.h"

#include <QDebug>
#include <QStringList>

EXPORT_PLUGIN(DebuggerPlugin)

const QString DebuggerPlugin::PLUGIN_NAME("debugger");
const int DebuggerPlugin::PLUGIN_VERSION(1);

QString DebuggerPlugin::name() const {
	return PLUGIN_NAME;
}

int DebuggerPlugin::version() const {
	return PLUGIN_VERSION;
}

void DebuggerPlugin::install(ScriptEngineInterface* engine) {
	debugger_ = new Debugger(engine);
	engine->setAgent(debugger_);
}

void DebuggerPlugin::uninstall() {
	if (debugger_) {
		delete debugger_;
	}
}

Debugger::Debugger(QScriptEngine* engine) :
		QScriptEngineAgent(engine) {
}

void Debugger::exceptionThrow(qint64 scriptId, const QScriptValue& exception, bool hasHandler) {
	if (!hasHandler) {
		qDebug() << "uncaught exception" << exception.toString();
		qDebug() << "backtrace:";
		foreach (const QString& value, engine()->uncaughtExceptionBacktrace()) {
			qDebug() << "\t" << value;
		}
	}
}
