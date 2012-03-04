/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
#include <QVariantMap>

#include <HookInterface.h>
#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>

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

	void install(HookInterface*) throw(Exception);
	void uninstall();

private:
	static QScriptValue findById(QScriptContext*, QScriptEngine*);
	static QScriptValue findByName(QScriptContext*, QScriptEngine*);

	QScriptEngine* engine_;
	QVariantMap config_;
	BattleListEntries* entries_;
};

class BattleListEntries: public QObject, public QScriptClass {
	Q_OBJECT

public:
	BattleListEntries(InternalBattleList::List* list, QScriptEngine* engine): QScriptClass(engine), list_(list) {}

	QScriptValue property(const QScriptValue&, const QScriptString&, uint);
	QueryFlags queryProperty(const QScriptValue&, const QScriptString&, QueryFlags, uint*);
	QScriptClassPropertyIterator* newIterator(const QScriptValue&);

	QScriptValue newBattleListEntry(QScriptEngine*, quint32);
	QScriptValue newBattleListEntry(QScriptEngine*, const InternalBattleList::Entry&);

	QScriptValue findById(QScriptEngine*, quint32);
	QScriptValue findByName(QScriptEngine*, const QString&);

private:
	InternalBattleList::List* list_;
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
