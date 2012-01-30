#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class MemoryModule: public Module {
public:
	QString name() const;

	void install();

private:
	static QScriptValue readU8(QScriptContext*, QScriptEngine*);
	static QScriptValue readU16(QScriptContext*, QScriptEngine*);
	static QScriptValue readU32(QScriptContext*, QScriptEngine*);
	static QScriptValue readString(QScriptContext*, QScriptEngine*);
};

#endif /* MEMORYMODULE_H */
