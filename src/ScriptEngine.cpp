#include "ScriptEngine.h"
#include "ReadOnlyPacketProxy.h"
#include "ReadWritePacketProxy.h"

#include <QDir>
#include <QFile>

ScriptEngine::ScriptEngine(Hook* hook) :
		ScriptEngineInterface(hook), hook_(hook) {
}

HookInterface* ScriptEngine::hook() {
	return hook_;
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
