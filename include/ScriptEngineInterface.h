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

#ifndef SCRIPTENGINEINTERFACE_H_
#define SCRIPTENGINEINTERFACE_H_

#include <QtGlobal>
#include <QByteArray>
#include <QScriptEngine>
#include <QString>

#include <SenderInterface.h>
#include <ReadOnlyPacketInterface.h>
#include <ReadWritePacketInterface.h>

class ScriptEngineInterface: public QScriptEngine {
public:
	ScriptEngineInterface(QObject* parent = 0): QScriptEngine(parent) {}
	virtual ~ScriptEngineInterface() {}

	virtual SenderInterface* sender() const = 0;

	virtual bool reload() = 0;
	virtual bool require(const QString&) = 0;

	virtual ReadOnlyPacketInterface* createReadOnlyPacket(const QByteArray&) = 0;
	virtual ReadOnlyPacketInterface* createReadOnlyPacket(const quint8*, quint16) = 0;
	virtual ReadWritePacketInterface* createReadWritePacket() = 0;
};

#endif
