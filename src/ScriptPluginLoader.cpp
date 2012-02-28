#include "ScriptPluginLoader.h"

ScriptPluginLoader::ScriptPluginLoader(const QString& path) :
		path_(path), instance_(NULL) {
}

ScriptPluginInterface* ScriptPluginLoader::instance() {
	typedef ScriptPluginInterface* (*LoadPrototype)();

	if (!instance_) {
		QLibrary library(path_);
		if (library.load()) {
			LoadPrototype loadFunction = (LoadPrototype) library.resolve(EXPORT_FUNCTION_STRING);
			if (loadFunction) {
				ScriptPluginInterface* plugin = loadFunction();
				instance_ = plugin;
			}
		}
	}
	return instance_;
}
