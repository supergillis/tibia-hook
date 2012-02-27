#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

class MemoryPlugin: public QObject, public ScriptPluginInterface {
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
	quint8 readU8(quint32);
	quint16 readU16(quint32);
	quint32 readU32(quint32);
	QString readString(quint32);

private:
	ScriptEngineInterface* engine_;
};

#endif /* MEMORYMODULE_H */
