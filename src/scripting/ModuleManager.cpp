#include "ModuleManager.h"

ModuleManager::ModuleManager(Hook* hook, QScriptEngine* engine) :
		hook_(hook), engine_(engine) {
}

Hook* ModuleManager::hook() {
	return hook_;
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
