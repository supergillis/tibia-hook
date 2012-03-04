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
#include "ScriptPluginLoader.h"
#include "ReadOnlyPacketProxy.h"
#include "ReadWritePacketProxy.h"

#include <QDir>
#include <QFile>

#ifdef Q_WS_WIN
#define PLUGIN_NAME "plugin.dll"
#else
#define PLUGIN_NAME "plugin.so"
#endif

ScriptEngine::ScriptEngine(SenderInterface* sender, QObject* parent) :
	ScriptEngineInterface(parent), sender_(sender) {
	receiveFromClientHandle_ = toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = toStringHandle("receiveFromServer");

	// Load extensions
    importExtension("qt.core");
    importExtension("qt.gui");

	// Load plugins
	QList<QFileInfo> pluginsInfo = QDir("plugins").entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo& pluginInfo, pluginsInfo) {
		ScriptPluginLoader loader(QDir(pluginInfo.absoluteFilePath()).filePath(PLUGIN_NAME));
		ScriptPluginInterface* plugin = loader.instance();
		if (plugin) {
			qDebug() << "installing" << plugin->name();
			plugin->install(this);
			plugins_.append(plugin);
		}
	}

	// Load main script
	reload();
}

ScriptEngine::~ScriptEngine() {
	foreach(ScriptPluginInterface* plugin, plugins_) {
		qDebug() << "uninstalling" << plugin->name();
		plugin->uninstall();
	}
}

SenderInterface* ScriptEngine::sender() const {
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

bool ScriptEngine::receiveFromClient(const QByteArray& data) {
	QScriptValue callback = globalObject().property(receiveFromClientHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(globalObject(), args << value);
		return result.isBool() ? result.toBool() : true;
	}
	return true;
}

bool ScriptEngine::receiveFromServer(const QByteArray& data) {
	QScriptValue callback = globalObject().property(receiveFromServerHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(globalObject(), args << value);
		return result.isBool() ? result.toBool() : true;
	}
	return true;
}
