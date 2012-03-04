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

#include "ScriptHook.h"
#include "ScriptPluginLoader.h"
#include "ReadOnlyPacketProxy.h"
#include "ReadWritePacketProxy.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#ifdef Q_WS_WIN
#define PLUGIN_NAME "plugin.dll"
#else
#define PLUGIN_NAME "plugin.so"
#endif

ScriptHook::ScriptHook(ConfigInterface* config, SenderInterface* sender, QObject* parent) :
	HookInterface(), config_(config), sender_(sender), engine_(parent) {
	receiveFromClientHandle_ = engine_.toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = engine_.toStringHandle("receiveFromServer");

	// Load scripting directory and main script
	QVariantMap scripting = config->value("scripting").toMap();
	if(!scripting.value("directory").isValid())
		throw Exception("Could not script directory!");
	else if(!scripting.value("script").isValid())
		throw Exception("Could not main script!");

	scriptDirectory_ = scripting.value("directory").toString();
	scriptMain_ = scripting.value("script").toString();

	// Load extensions
	engine_.importExtension("qt.core");
	engine_.importExtension("qt.gui");

	// Load plugins
	QList<QFileInfo> pluginsInfo = QDir("plugins").entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo& pluginInfo, pluginsInfo) {
		ScriptPluginLoader loader(QDir(pluginInfo.absoluteFilePath()).filePath(PLUGIN_NAME));
		ScriptPluginInterface* plugin = loader.instance();
		if (plugin) {
			qDebug() << "installing" << plugin->name();
			try {
				plugin->install(this);
				plugins_.append(plugin);
			}
			catch(Exception& exception) {
				QMessageBox message;
				message.setWindowTitle(QApplication::applicationName());
				message.setText("Could not load \"" + plugin->name() + "\" plugin!");
				message.setDetailedText(exception.message());
				message.setDefaultButton(QMessageBox::Ignore);
				message.exec();
			}
		}
	}

	// Load main script
	reload();
}

ScriptHook::~ScriptHook() {
	foreach(ScriptPluginInterface* plugin, plugins_) {
		qDebug() << "uninstalling" << plugin->name();
		plugin->uninstall();
	}
}

QScriptEngine* ScriptHook::engine() {
	return &engine_;
}

ConfigInterface* ScriptHook::config() {
	return config_;
}

SenderInterface* ScriptHook::sender() {
	return sender_;
}

ReceiverInterface* ScriptHook::receiver() {
	return this;
}

bool ScriptHook::reload() {
	engine_.pushContext();
	requiredFiles_.clear();
	require(scriptMain_);
	engine_.popContext();
	return true;
}

bool ScriptHook::require(const QString& path) {
	QDir scripts(QDir::current());
	if (scripts.cd(scriptDirectory_)) {
		QString fileName = scripts.absoluteFilePath(path);
		QFile file(fileName);
		if (!requiredFiles_.contains(fileName)) {
			if (file.open(QFile::ReadOnly)) {
				engine_.evaluate(file.readAll(), fileName);
				requiredFiles_ << fileName;
				return true;
			}
		}
	}
	return false;
}

ReadOnlyPacketInterface* ScriptHook::createReadOnlyPacket(const QByteArray& data) {
	return new ReadOnlyPacketProxy(data);
}

ReadOnlyPacketInterface* ScriptHook::createReadOnlyPacket(const quint8* data, quint16 length) {
	return new ReadOnlyPacketProxy(data, length);
}

ReadWritePacketInterface* ScriptHook::createReadWritePacket() {
	return new ReadWritePacketProxy();
}

bool ScriptHook::receiveFromClient(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromClientHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(engine_.globalObject(), args << value);
		return result.isBool() ? result.toBool() : true;
	}
	return true;
}

bool ScriptHook::receiveFromServer(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromServerHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(engine_.globalObject(), args << value);
		return result.isBool() ? result.toBool() : true;
	}
	return true;
}
