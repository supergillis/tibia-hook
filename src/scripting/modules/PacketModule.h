#ifndef PACKETMODULE_H
#define PACKETMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class PacketModule: public Module {
public:
	QString name() const;

	void install();

private:
	static QScriptValue constructor(QScriptContext*, QScriptEngine*);

	static QScriptValue readU8(QScriptContext*, QScriptEngine*);
	static QScriptValue readU16(QScriptContext*, QScriptEngine*);
	static QScriptValue readU32(QScriptContext*, QScriptEngine*);
	static QScriptValue readString(QScriptContext*, QScriptEngine*);

	static QScriptValue writeU8(QScriptContext*, QScriptEngine*);
	static QScriptValue writeU16(QScriptContext*, QScriptEngine*);
	static QScriptValue writeU32(QScriptContext*, QScriptEngine*);
	static QScriptValue writeString(QScriptContext*, QScriptEngine*);
};

#endif /* PACKETMODULE_H */
