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
