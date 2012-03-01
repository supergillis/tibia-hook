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

#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include <QByteArray>
#include <QDir>
#include <QList>
#include <QObject>
#include <QScriptEngine>
#include <QScriptString>

#include <ScriptPluginInterface.h>

#include "Handler.h"
#include "Hook.h"
#include "ScriptEngine.h"

class ScriptHandler: public Handler {
public:
	ScriptHandler(Hook*);
	virtual ~ScriptHandler();

	void reload();
	void install(ScriptPluginInterface*);

	bool receiveFromClient(const QByteArray&);
	void receiveFromServer(const QByteArray&);

private:
	ScriptEngine engine_;

	QList<ScriptPluginInterface*> plugins_;

	QScriptString receiveFromClientHandle_;
	QScriptString receiveFromServerHandle_;
};

#endif /* SCRIPTHANDLER_H_ */
