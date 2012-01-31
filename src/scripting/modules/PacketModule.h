#ifndef PACKETMODULE_H
#define PACKETMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ClassModule.h"
#include "ScriptHandler.h"
#include "DecryptedMessage.h"

class PacketModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;

	PacketModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

	QScriptValue createReadOnlyPacket(const DecryptedMessage*);
	QScriptValue createReadWritePacket();

private:
	ClassModule* classModule_;
	QScriptEngine* engine_;
	QScriptValue packetClass_;

	static QScriptValue constructor(QScriptContext*, QScriptEngine*);

	static QScriptValue readU8(QScriptContext*, QScriptEngine*);
	static QScriptValue readU16(QScriptContext*, QScriptEngine*);
	static QScriptValue readU32(QScriptContext*, QScriptEngine*);
	static QScriptValue readString(QScriptContext*, QScriptEngine*);

	static QScriptValue writeU8(QScriptContext*, QScriptEngine*);
	static QScriptValue writeU16(QScriptContext*, QScriptEngine*);
	static QScriptValue writeU32(QScriptContext*, QScriptEngine*);
	static QScriptValue writeString(QScriptContext*, QScriptEngine*);

	static const QString VARIABLE_NAME;
};

#endif /* PACKETMODULE_H */
