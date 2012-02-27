#ifndef SCRIPTENGINEINTERFACE_H_
#define SCRIPTENGINEINTERFACE_H_

#include <QtGlobal>
#include <QByteArray>
#include <QString>

#include <HookInterface.h>
#include <ReadOnlyPacketInterface.h>
#include <ReadWritePacketInterface.h>

class ScriptEngineInterface: public QScriptEngine {
public:
	ScriptEngineInterface(QObject* parent = 0): QScriptEngine(parent) {}
	virtual ~ScriptEngineInterface() {}

	virtual HookInterface* hook() = 0;

	virtual bool reload() = 0;
	virtual bool require(const QString&) = 0;

	virtual ReadOnlyPacketInterface* createReadOnlyPacket(const QByteArray&) = 0;
	virtual ReadOnlyPacketInterface* createReadOnlyPacket(const quint8*, quint16) = 0;
	virtual ReadWritePacketInterface* createReadWritePacket() = 0;
};

#endif
