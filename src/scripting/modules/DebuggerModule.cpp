#include <QDebug>

#include "DebuggerModule.h"

DebuggerModule::DebuggerModule() :
		debugger_(NULL) {
}

DebuggerModule::~DebuggerModule() {
	if (debugger_) {
		delete debugger_;
	}
}

QString DebuggerModule::name() const {
	return "debugger";
}

void DebuggerModule::install() {
	QScriptEngine* engine = handler()->engine();
	debugger_ = new Debugger(engine);
	engine->setAgent(debugger_);
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
