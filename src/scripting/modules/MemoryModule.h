#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H

#include <QObject>
#include <QScriptEngine>
#include <QScriptContext>

#include "Module.h"
#include "ModuleManager.h"

class MemoryModule: public Module {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const QString VARIABLE_NAME;

	MemoryModule(QObject*);

	QString name() const;

	bool install(ModuleManager*);

public slots:
	quint8 readU8(quint32);
	quint16 readU16(quint32);
	quint32 readU32(quint32);
	QString readString(quint32);
};

#endif /* MEMORYMODULE_H */
