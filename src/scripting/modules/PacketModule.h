#ifndef PACKETMODULE_H
#define PACKETMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ModuleManager.h"

class PacketModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const QString VARIABLE_NAME;

	PacketModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

private:
	static QScriptValue constructor(QScriptContext*, QScriptEngine*);
};

#endif /* PACKETMODULE_H */
