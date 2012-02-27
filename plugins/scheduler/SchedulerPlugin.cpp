#include "SchedulerPlugin.h"

EXPORT_PLUGIN(SchedulerPlugin)

const QString SchedulerPlugin::PLUGIN_NAME("scheduler");
const int SchedulerPlugin::PLUGIN_VERSION(1);

const QString SchedulerPlugin::VARIABLE_NAME("Scheduler");

QString SchedulerPlugin::name() const {
	return PLUGIN_NAME;
}

int SchedulerPlugin::version() const {
	return PLUGIN_VERSION;
}

void SchedulerPlugin::install(ScriptEngineInterface* engine) {
	engine_ = engine;
	engine_->globalObject().setProperty(VARIABLE_NAME, engine_->newQObject(this), QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void SchedulerPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
}

int SchedulerPlugin::start(QScriptValue callback, quint32 interval) {
	if (callback.isFunction()) {
		int timerId = startTimer(interval);
		if (timerId > 0) {
			callbacks_.insert(timerId, QPair<bool, QScriptValue>(false, callback));
			return timerId;
		}
	}
	return 0;
}

int SchedulerPlugin::single(QScriptValue callback, quint32 interval) {
	if (callback.isFunction()) {
		int timerId = startTimer(interval);
		if (timerId > 0) {
			callbacks_.insert(timerId, QPair<bool, QScriptValue>(true, callback));
			return timerId;
		}
	}
	return 0;
}

void SchedulerPlugin::stop(quint32 timerId) {
	killTimer(timerId);
	callbacks_.remove(timerId);
}

void SchedulerPlugin::timerEvent(QTimerEvent* event) {
	int timerId = event->timerId();
	QPair<bool, QScriptValue> entry = callbacks_.value(timerId);
	QScriptValue callback = entry.second;
	if (callback.isValid()) {
		callback.call();
	}
	// If this entry is a single shot entry, then we stop that timer
	if (entry.first) {
		stop(timerId);
	}
}
