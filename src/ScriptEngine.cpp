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

#include "ScriptEngine.h"
#include "ReadOnlyPacketProxy.h"
#include "ReadWritePacketProxy.h"

#include <QDir>
#include <QFile>

ScriptEngine::ScriptEngine(SenderInterface* sender, QObject* parent) :
	ScriptEngineInterface(parent), sender_(sender) {
    importExtension("qt.core");
    importExtension("qt.gui");
}

SenderInterface* ScriptEngine::sender() {
	return sender_;
}

bool ScriptEngine::reload() {
	pushContext();
	requiredFiles_.clear();
	require("Main.js");
	popContext();
	return true;
}

bool ScriptEngine::require(const QString& path) {
	QDir scripts(QDir::current());
	if (scripts.cd("scripts")) {
		QString fileName = scripts.absoluteFilePath(path);
		QFile file(fileName);
		if (!requiredFiles_.contains(fileName)) {
			if (file.open(QFile::ReadOnly)) {
				evaluate(file.readAll(), fileName);
				requiredFiles_ << fileName;
				return true;
			}
		}
	}
	return false;
}

ReadOnlyPacketInterface* ScriptEngine::createReadOnlyPacket(const QByteArray& data) {
	return new ReadOnlyPacketProxy(data);
}

ReadOnlyPacketInterface* ScriptEngine::createReadOnlyPacket(const quint8* data, quint16 length) {
	return new ReadOnlyPacketProxy(data, length);
}

ReadWritePacketInterface* ScriptEngine::createReadWritePacket() {
	return new ReadWritePacketProxy();
}
