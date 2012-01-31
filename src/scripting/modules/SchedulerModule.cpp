#include "SchedulerModule.h"

const QString SchedulerModule::PLUGIN_NAME("scheduler");
const QString SchedulerModule::VARIABLE_NAME("Scheduler");

SchedulerModule::SchedulerModule(QObject* parent) :
		Module(parent) {
}

QString SchedulerModule::name() const {
	return PLUGIN_NAME;
}

bool SchedulerModule::install(ModuleManager* manager) {
	QScriptEngine* engine = manager->engine();
	QScriptValue schedulerObject = engine->newQObject(this);
	engine->globalObject().setProperty(VARIABLE_NAME, schedulerObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

int SchedulerModule::start(QScriptValue callback, quint32 interval) {
	if (callback.isFunction()) {
		int timerId = startTimer(interval);
		if (timerId > 0) {
			callbacks_.insert(timerId, callback);
			return timerId;
		}
	}
	return 0;
}

void SchedulerModule::stop(quint32 timerId) {
	killTimer(timerId);
	callbacks_.remove(timerId);
}

void SchedulerModule::timerEvent(QTimerEvent* event) {
	QScriptValue callback = callbacks_.value(event->timerId(), QScriptValue());
	if (callback.isValid()) {
		callback.call();
	}
}
