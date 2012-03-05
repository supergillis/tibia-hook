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

#ifndef SCRIPTHOOK_H_
#define SCRIPTHOOK_H_

#include <QScriptEngine>
#include <QStringList>

#include <ConfigInterface.h>
#include <HookInterface.h>
#include <ScriptPluginInterface.h>
#include <SenderInterface.h>
#include <ReadOnlyPacketInterface.h>
#include <ReadWritePacketInterface.h>
#include <ReceiverInterface.h>

class ScriptHook: public HookInterface, public ReceiverInterface {
public:
	ScriptHook(ConfigInterface*, SenderInterface*, QObject* = 0);
	~ScriptHook();

	QScriptEngine* engine();

	ConfigInterface* config();

	SenderInterface* sender();
	ReceiverInterface* receiver();

	bool reload();
	bool require(const QString&);

	ReadOnlyPacketInterface* createReadOnlyPacket(const QByteArray&);
	ReadOnlyPacketInterface* createReadOnlyPacket(const quint8*, quint16);
	ReadWritePacketInterface* createReadWritePacket();

	bool receiveFromClient(const QByteArray&);
	bool receiveFromServer(const QByteArray&);

private:
	ConfigInterface* config_;
	SenderInterface* sender_;

	QString scriptDirectory_;
	QString scriptMain_;

	QScriptEngine engine_;
	QScriptString receiveFromClientHandle_;
	QScriptString receiveFromServerHandle_;

	QStringList requiredFiles_;
	QList<ScriptPluginInterface*> plugins_;
};

#endif
