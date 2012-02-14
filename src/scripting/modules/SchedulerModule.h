#ifndef SCHEDULERMODULE_H
#define SCHEDULERMODULE_H

#include <QHash>
#include <QPair>
#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>
#include <QTimerEvent>

#include "Module.h"
#include "ModuleManager.h"
#include "Hook.h"
#include "Packet.h"

class SchedulerModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const QString VARIABLE_NAME;

	SchedulerModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

public slots:
	int start(QScriptValue, quint32);
	int single(QScriptValue, quint32);
	void stop(quint32);

protected:
	void timerEvent(QTimerEvent*);

private:
	QHash<int, QPair<bool, QScriptValue> > callbacks_;
};

#endif /* SCHEDULERMODULE_H */
