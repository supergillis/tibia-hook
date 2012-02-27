#ifndef CLASSPLUGIN_H
#define CLASSPLUGIN_H

#include <QObject>
#include <QString>
#include <QScriptContext>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class ClassPlugin: public QObject, public ScriptPluginInterface {
Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	QString name() const;
	int version() const;

	void install(ScriptEngineInterface*);
	void uninstall();

	QScriptValue extendRootClassWithArgs(QScriptValue);
	QScriptValue extendRootClass();
	QScriptValue extendClassWithArgs(QScriptValue, QScriptValue);
	QScriptValue extendClass(QScriptValue);

	QScriptValue createRootInstanceWithArgs(QScriptValue);
	QScriptValue createRootInstance();
	QScriptValue createInstanceWithArgs(QScriptValue, QScriptValue);
	QScriptValue createInstance(QScriptValue);

private:
	QScriptValue createClassPrototype();
	QScriptValue createInstancePrototype();

	ScriptEngineInterface* engine_;

	QScriptValue rootClass_;

	QScriptString instanceHandle_;
	QScriptString extendHandle_;
	QScriptString extendedHandle_;
	QScriptString createHandle_;
	QScriptString constructorHandle_;

	static QScriptValue extend(QScriptContext*, QScriptEngine*);
	static QScriptValue extended(QScriptContext*, QScriptEngine*);
	static QScriptValue create(QScriptContext*, QScriptEngine*);
	static QScriptValue constructor(QScriptContext*, QScriptEngine*);

	static const QString VARIABLE_NAME;
};

#endif /* CLASSPLUGIN_H */
