#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <QScriptEngine>

#include "Module.h"

class ModuleManager {
public:
	ModuleManager(QScriptEngine*);

	QScriptEngine* engine();

	bool install(Module*);

	Module* lookup(const QString&);

private:
	QScriptEngine* engine_;
	QHash<QString, Module*> modules_;
};

#endif /* MODULEMANAGER_H_ */
