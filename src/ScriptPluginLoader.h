#ifndef SCRIPTPLUGINLOADER_H_
#define SCRIPTPLUGINLOADER_H_

#include <QLibrary>
#include <QPointer>
#include <QString>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>

class ScriptPluginLoader {
public:
	ScriptPluginLoader(const QString&);
	virtual ~ScriptPluginLoader() {}

	ScriptPluginInterface* instance();

private:
	QString path_;
	ScriptPluginInterface* instance_;
};

#endif /* SCRIPTPLUGINLOADER_H_ */
