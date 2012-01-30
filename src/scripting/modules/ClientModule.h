#ifndef CLIENTMODULE_H
#define CLIENTMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class ClientModule: public Module {
public:
	QString name() const;

	void install();

private:
	static QScriptValue sendPacket(QScriptContext*, QScriptEngine*);
	static QScriptValue sendKeyPress(QScriptContext*, QScriptEngine*);
};

#endif /* CLIENTMODULE_H */
