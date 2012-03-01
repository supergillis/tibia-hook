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

#ifndef SCRIPTENGINE_H_
#define SCRIPTENGINE_H_

#include <QScriptEngine>

#include <ScriptEngineInterface.h>
#include <ReadOnlyPacketInterface.h>
#include <ReadWritePacketInterface.h>

#include "Hook.h"

class ScriptEngine: public ScriptEngineInterface {
public:
	ScriptEngine(Hook*);
	virtual ~ScriptEngine() {}

	HookInterface* hook();

	bool reload();
	bool require(const QString&);

	ReadOnlyPacketInterface* createReadOnlyPacket(const QByteArray&);
	ReadOnlyPacketInterface* createReadOnlyPacket(const quint8*, quint16);
	ReadWritePacketInterface* createReadWritePacket();

private:
	Hook* hook_;

	QStringList requiredFiles_;
};

#endif /* SCRIPTENGINE_H_ */
