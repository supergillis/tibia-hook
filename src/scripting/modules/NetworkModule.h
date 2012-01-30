#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ScriptHandler.h"

class NetworkModule: public Module {
public:
	QString name() const;

	void install();

private:
	static QScriptValue sendToServer(QScriptContext*, QScriptEngine*);
	static QScriptValue sendToClient(QScriptContext*, QScriptEngine*);
};

#endif /* NETWORKMODULE_H */
