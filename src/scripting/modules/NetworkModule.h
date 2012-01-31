#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ModuleManager.h"
#include "Hook.h"
#include "Packet.h"

class NetworkModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const QString VARIABLE_NAME;

	NetworkModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

public slots:
	void sendToServer(Packet*);
	void sendToClient(Packet*);

private:
	Hook* hook_;
};

#endif /* NETWORKMODULE_H */
