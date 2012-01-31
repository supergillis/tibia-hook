#ifndef CLIENTMODULE_H
#define CLIENTMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ModuleManager.h"
#include "Packet.h"
#include "Hook.h"

class ClientModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const QString VARIABLE_NAME;

	ClientModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

public slots:
	void sendPacket(Packet*);
	void sendKeyPress(int);

private:
	Hook* hook_;
};

#endif /* CLIENTMODULE_H */
