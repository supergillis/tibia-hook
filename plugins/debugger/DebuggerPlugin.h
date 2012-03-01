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

#ifndef DEBUGGER_MODULE_H
#define DEBUGGER_MODULE_H

#include <QScriptEngine>
#include <QScriptEngineAgent>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class Debugger;
class DebuggerPlugin: public QObject, public ScriptPluginInterface {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	QString name() const;
	int version() const;

	void install(ScriptEngineInterface*);
	void uninstall();

private:
	Debugger* debugger_;
};

class Debugger: public QScriptEngineAgent {
public:
	Debugger(QScriptEngine*);

	void exceptionThrow(qint64, const QScriptValue&, bool);
};

#endif /* DEBUGGER_MODULE_H */
