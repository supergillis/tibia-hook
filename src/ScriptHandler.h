#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include <QByteArray>
#include <QDir>
#include <QList>
#include <QObject>
#include <QScriptEngine>
#include <QScriptString>

#include <ScriptPluginInterface.h>

#include "Handler.h"
#include "Hook.h"
#include "ScriptEngine.h"

class ScriptHandler: public Handler {
public:
	ScriptHandler(Hook*);
	virtual ~ScriptHandler();

	bool receiveFromClient(const QByteArray&);
	void receiveFromServer(const QByteArray&);

private:
	void loadPlugins(const QDir&, QList<ScriptPluginInterface*>&);

	ScriptEngine engine_;

	QList<ScriptPluginInterface*> plugins_;

	QScriptString receiveFromClientHandle_;
	QScriptString receiveFromServerHandle_;
};

#endif /* SCRIPTHANDLER_H_ */
