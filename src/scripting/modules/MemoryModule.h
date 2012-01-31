#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class MemoryModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;

	MemoryModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

private:
	static QScriptValue readU8(QScriptContext*, QScriptEngine*);
	static QScriptValue readU16(QScriptContext*, QScriptEngine*);
	static QScriptValue readU32(QScriptContext*, QScriptEngine*);
	static QScriptValue readString(QScriptContext*, QScriptEngine*);

	static const QString VARIABLE_NAME;
};

#endif /* MEMORYMODULE_H */
