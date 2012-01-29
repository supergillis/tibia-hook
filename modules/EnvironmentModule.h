#ifndef ENVIRONMENTMODULE_H_
#define ENVIRONMENTMODULE_H_

#include "Module.h"

#include <QObject>
#include <QScriptEngine>

class EnvironmentModule: public Module {
	Q_OBJECT

public:
	EnvironmentModule(QObject*);

	void install(QScriptEngine*);

};

#endif /* ENVIRONMENTMODULE_H_ */
