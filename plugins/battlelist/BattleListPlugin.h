#ifndef SCHEDULERMODULE_H
#define SCHEDULERMODULE_H

#include <QHash>
#include <QPair>
#include <QObject>
#include <QScriptClass>
#include <QScriptClassPropertyIterator>
#include <QScriptContext>
#include <QScriptEngine>
#include <QTimerEvent>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>
#include <ScriptEngineInterface.h>

#include "InternalBattleList.h"

class BattleListEntries;
class BattleListPlugin: public QObject, public ScriptPluginInterface {
	Q_OBJECT

public:
	static const QString PLUGIN_NAME;
	static const int PLUGIN_VERSION;

	static const QString VARIABLE_NAME;

	QString name() const;
	int version() const;

	void install(ScriptEngineInterface*);
	void uninstall();

	static QScriptValue newBattleListEntry(QScriptEngine*, quint32);

private:
	static QScriptValue newBattleListEntry(QScriptEngine*, const InternalBattleList::Entry&);

	static QScriptValue findById(QScriptEngine*, quint32);
	static QScriptValue findByName(QScriptEngine*, const QString&);

	static QScriptValue findById(QScriptContext*, QScriptEngine*);
	static QScriptValue findByName(QScriptContext*, QScriptEngine*);

	static const InternalBattleList::List* battleList;

	QScriptEngine* engine_;
	BattleListEntries* entries_;
};

class BattleListEntries: public QScriptClass {
public:
	BattleListEntries(QScriptEngine* engine): QScriptClass(engine) {}

	QScriptValue property(const QScriptValue&, const QScriptString&, uint);
	QueryFlags queryProperty(const QScriptValue&, const QScriptString&, QueryFlags, uint*);
	QScriptClassPropertyIterator* newIterator(const QScriptValue&);
};

class BattleListEntriesIterator: public QScriptClassPropertyIterator {
public:
	BattleListEntriesIterator(const QScriptValue&);

	QScriptString name() const;
	uint id() const;

	bool hasNext() const;
	bool hasPrevious() const;

	void next();
	void previous();

	void toBack();
	void toFront();

private:
	uint index_;
	uint last_;
};

#endif /* SCHEDULERMODULE_H */
