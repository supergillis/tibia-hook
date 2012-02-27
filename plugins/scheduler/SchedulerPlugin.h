#ifndef SCHEDULERMODULE_H
#define SCHEDULERMODULE_H

#include <QHash>
#include <QPair>
#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>
#include <QTimerEvent>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class SchedulerPlugin: public QObject, public ScriptPluginInterface {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	static const QString VARIABLE_NAME;

	QString name() const;
	int version() const;

	void install(ScriptEngineInterface*);
	void uninstall();

public slots:
	int start(QScriptValue, quint32);
	int single(QScriptValue, quint32);
	void stop(quint32);

protected:
	void timerEvent(QTimerEvent*);

private:
	ScriptEngineInterface* engine_;
	QHash<int, QPair<bool, QScriptValue> > callbacks_;
};

#endif /* SCHEDULERMODULE_H */
