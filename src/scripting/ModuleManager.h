#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <QScriptEngine>

#include "Hook.h"
#include "Module.h"

class ModuleManager {
public:
	ModuleManager(Hook*, QScriptEngine*);

	Hook* hook();
	QScriptEngine* engine();

	bool install(Module*);

	Module* lookup(const QString&);

private:
	Hook* hook_;
	QScriptEngine* engine_;
	QHash<QString, Module*> modules_;
};

#endif /* MODULEMANAGER_H_ */
