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

#ifndef SCRIPTRECEIVER_H_
#define SCRIPTRECEIVER_H_

#include <QByteArray>
#include <QDir>
#include <QList>
#include <QObject>
#include <QScriptEngine>
#include <QScriptString>

#include <ReceiverInterface.h>
#include <ScriptPluginInterface.h>
#include <SenderInterface.h>

#include "ScriptEngine.h"

class ScriptReceiver: public ReceiverInterface {
public:
	ScriptReceiver(SenderInterface*, QObject* = 0);
	virtual ~ScriptReceiver();

	void reload();
	void install(ScriptPluginInterface*);

	bool receiveFromClient(const QByteArray&);
	bool receiveFromServer(const QByteArray&);

private:
	ScriptEngine engine_;

	QList<ScriptPluginInterface*> plugins_;

	QScriptString receiveFromClientHandle_;
	QScriptString receiveFromServerHandle_;
};

#endif
