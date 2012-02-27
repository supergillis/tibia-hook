#include "ScriptHandler.h"
#include "ReadOnlyPacketProxy.h"

#include <QFileInfo>
#include <QLibrary>

typedef ScriptPluginInterface* (*LoadPrototype)();

ScriptHandler::ScriptHandler(Hook* hook) :
		Handler(), engine_(hook) {
	receiveFromClientHandle_ = engine_.toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = engine_.toStringHandle("receiveFromServer");

	loadPlugins(QDir("plugins"), plugins_);

	foreach(ScriptPluginInterface* plugin, plugins_) {
		plugin->install(&engine_);
		qDebug() << "installed plugin" << plugin->name();
	}

	engine_.reload();
}

ScriptHandler::~ScriptHandler() {
	foreach(ScriptPluginInterface* plugin, plugins_) {
		qDebug() << "uninstalling plugin" << plugin->name();
		plugin->uninstall();
	}
}

void ScriptHandler::loadPlugins(const QDir& directory, QList<
        ScriptPluginInterface*>& plugins) {
	QList<QFileInfo> pluginsInfo = directory.entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo& pluginInfo, pluginsInfo) {
		QDir pluginDir(pluginInfo.absoluteFilePath());
		if (!pluginDir.exists("plugin.so")) {
			qDebug() << "no plugin.so found in" << pluginDir.absolutePath();
			continue;
		}
		QLibrary library(pluginDir.filePath("plugin.so"));
		if (!library.load()) {
			qDebug() << library.errorString();
			continue;
		}
		LoadPrototype loadFunction = (LoadPrototype) library.resolve("load");
		if (!loadFunction) {
			qDebug() << "could not resolve \"load\" function";
			continue;
		}
		plugins.append(loadFunction());
	}
}

bool ScriptHandler::receiveFromClient(const QByteArray& data) {
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

void ScriptHandler::receiveFromServer(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromServerHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		callback.call(engine_.globalObject(), args << value);
	}
}
