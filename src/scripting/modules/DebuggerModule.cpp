#include "DebuggerModule.h"

const QString DebuggerModule::PLUGIN_NAME("debugger");

DebuggerModule::DebuggerModule(QObject* parent) :
		Module(parent), debugger_(NULL) {
}

DebuggerModule::~DebuggerModule() {
	if (debugger_) {
		delete debugger_;
	}
}

QString DebuggerModule::name() const {
	return PLUGIN_NAME;
}

bool DebuggerModule::install(ModuleManager* manager) {
	QScriptEngine* engine = manager->engine();
	debugger_ = new Debugger(engine);
	engine->setAgent(debugger_);
	return true;
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
