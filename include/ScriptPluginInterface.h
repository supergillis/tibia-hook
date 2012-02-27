#ifndef SCRIPTPLUGININTERFACE_H_
#define SCRIPTPLUGININTERFACE_H_

#include <QString>
#include <QScriptEngine>

#include <ScriptEngineInterface.h>

class ScriptPluginInterface {
public:
	virtual ~ScriptPluginInterface() {}

	virtual QString name() const = 0;
	virtual int version() const = 0;

	virtual void install(ScriptEngineInterface*) = 0;
	virtual void uninstall() = 0;
};

#endif
