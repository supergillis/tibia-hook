#include "Module.h"
#include "ScriptHandler.h"

bool Module::install(ScriptHandler* handler) {
	if (handler_ == NULL) {
		handler_ = handler;
		install();
		return true;
	}
	return false;
}

ScriptHandler* Module::handler() const {
	return handler_;
}
