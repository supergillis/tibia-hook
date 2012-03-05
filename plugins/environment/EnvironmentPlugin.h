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

#ifndef ENVIRONMENTPLUGIN_H
#define ENVIRONMENTPLUGIN_H

#include <QFile>
#include <QObject>
#include <QStringList>
#include <QScriptEngine>
#include <QScriptContext>

#include <HookInterface.h>
#include <ScriptPluginInterface.h>

class EnvironmentPlugin: public QObject, public ScriptPluginInterface {
	Q_OBJECT
	Q_INTERFACES(ScriptPluginInterface)

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	static const QString VARIABLE_NAME;

	QString name() const;
	int version() const;

	void install(HookInterface*) throw(Exception);
	void uninstall();

public slots:
	bool reload();
	bool require(const QString&);

private:
	HookInterface* hook_;
	QScriptEngine* engine_;
};

#endif /* ENVIRONMENTPLUGIN_H */
