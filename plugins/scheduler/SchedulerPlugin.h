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

#ifndef SCHEDULERMODULE_H
#define SCHEDULERMODULE_H

#include <QHash>
#include <QPair>
#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>
#include <QTimerEvent>

#include <HookInterface.h>
#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>

class SchedulerPlugin: public QObject, public ScriptPluginInterface {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	static const QString VARIABLE_NAME;

	QString name() const;
	int version() const;

	void install(HookInterface*);
	void uninstall();

public slots:
	int start(QScriptValue, quint32);
	int single(QScriptValue, quint32);
	void stop(quint32);

protected:
	void timerEvent(QTimerEvent*);

private:
	QScriptEngine* engine_;
	QHash<int, QPair<bool, QScriptValue> > callbacks_;
};

#endif /* SCHEDULERMODULE_H */
