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

private:
	static QScriptValue constructor(QScriptContext*, QScriptEngine*);
	static QScriptValue readU8(QScriptContext*, QScriptEngine*);
	static QScriptValue readU16(QScriptContext*, QScriptEngine*);
	static QScriptValue readU32(QScriptContext*, QScriptEngine*);
	static QScriptValue readString(QScriptContext*, QScriptEngine*);
};

class MemoryObject: public QObject {
	Q_OBJECT
	Q_PROPERTY(quint16 position WRITE setPosition READ position)

public:
	MemoryObject(quint32 address, QObject* = 0);

	quint16 position() const;
	void setPosition(quint16);

public slots:
	void skip(quint16 = 1);

	quint8 readU8();
	quint16 readU16();
	quint32 readU32();
	QString readString(quint16);

private:
	quint32 address_;
	quint16 position_;
};

#endif /* MEMORYMODULE_H */
