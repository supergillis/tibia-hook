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
		qDebug() << "uncaught exception" << exception.toString() << "on line" << engine()->uncaughtExceptionLineNumber();
		qDebug() << "backtrace:";
		foreach (const QString& value, engine()->uncaughtExceptionBacktrace()) {
			qDebug() << "\t" << value;
		}
	}
}
