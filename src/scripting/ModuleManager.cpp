#include "ModuleManager.h"

ModuleManager::ModuleManager(QScriptEngine* engine) :
		engine_(engine) {
}

QScriptEngine* ModuleManager::engine() {
	return engine_;
}

bool ModuleManager::install(Module* module) {
	if (module->install(this)) {
		modules_.insert(module->name(), module);
		return true;
	}
	return false;
}

Module* ModuleManager::lookup(const QString& name) {
	return modules_.value(name, NULL);
}
