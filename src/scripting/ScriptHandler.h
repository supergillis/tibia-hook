#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include <QObject>
#include <QScriptEngine>
#include <QScriptString>

#include "Handler.h"
#include "ModuleManager.h"
#include "Hook.h"

class ScriptHandler: public QObject, public Handler {
public:
	ScriptHandler(Hook*);
	~ScriptHandler();

	Hook* hook();
	QScriptEngine* engine();

	void install(Module*);
	void reload();

	bool receiveFromClient(const QByteArray&);
	void receiveFromServer(const QByteArray&);

private:
	Hook* hook_;
	ModuleManager* moduleManager_;

	QScriptEngine engine_;
	QScriptString receiveFromClientHandle_;
	QScriptString receiveFromServerHandle_;
};

#endif /* SCRIPTHANDLER_H_ */
