/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SchedulerPlugin.h"

#include <QtPlugin>
Q_EXPORT_PLUGIN2(scheduler, SchedulerPlugin)

const QString SchedulerPlugin::PLUGIN_NAME("scheduler");
const int SchedulerPlugin::PLUGIN_VERSION(1);

const QString SchedulerPlugin::VARIABLE_NAME("Scheduler");

QString SchedulerPlugin::name() const {
	return PLUGIN_NAME;
}

int SchedulerPlugin::version() const {
	return PLUGIN_VERSION;
}

void SchedulerPlugin::install(HookInterface* hook) throw(Exception) {
	engine_ = hook->engine();
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
