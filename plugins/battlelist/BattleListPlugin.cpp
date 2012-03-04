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

#include "BattleListPlugin.h"

#include <Exception.h>

EXPORT_PLUGIN(BattleListPlugin)

const QString BattleListPlugin::PLUGIN_NAME("battlelist");
const int BattleListPlugin::PLUGIN_VERSION(1);

const QString BattleListPlugin::VARIABLE_NAME("BattleList");

QString BattleListPlugin::name() const {
	return PLUGIN_NAME;
}

int BattleListPlugin::version() const {
	return PLUGIN_VERSION;
}

void BattleListPlugin::install(HookInterface* hook) throw(Exception) {
	engine_ = hook->engine();
	config_ = hook->config()->value(PLUGIN_NAME).toMap();
	if(!config_.value("address").isValid())
		throw Exception("Could not load battlelist address!");

	InternalBattleList::List* list = (InternalBattleList::List*) config_.value("address").toUInt();
	entries_ = new BattleListEntries(list, engine_);
	QScriptValue entriesObject = engine_->newObject(entries_);
	QScriptValue findById = engine_->newFunction(BattleListPlugin::findById);
	QScriptValue findByName = engine_->newFunction(BattleListPlugin::findByName);
	findById.setData(entriesObject);
	findByName.setData(entriesObject);

	QScriptValue object = engine_->newObject();
	object.setData(engine_->newQObject(this));
	object.setProperty("entries", entriesObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	object.setProperty("findById", findById, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	object.setProperty("findByName", findByName, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	engine_->globalObject().setProperty(VARIABLE_NAME, object, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void BattleListPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	delete entries_;
	entries_ = NULL;
	engine_ = NULL;
}

QScriptValue BattleListPlugin::findById(QScriptContext* context, QScriptEngine* engine) {
	if(context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if(value.isNumber()) {
			quint32 id = value.toUInt32();
			BattleListEntries* entries = qobject_cast<BattleListEntries*>(context->callee().data().toQObject());
			return entries->findById(engine, id);
		}
	}
	return context->throwError("invalid call to findById(Number)");
}

QScriptValue BattleListPlugin::findByName(QScriptContext* context, QScriptEngine* engine) {
	if(context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if(value.isString()) {
			QString name = value.toString();
			BattleListEntries* entries = qobject_cast<BattleListEntries*>(context->callee().data().toQObject());
			return entries->findByName(engine, name);
		}
	}
	return context->throwError("invalid call to findByName(String)");
}

QScriptValue BattleListEntries::newBattleListEntry(QScriptEngine* engine, quint32 index) {
	if(index < BATTLELIST_LENGTH) {
		return newBattleListEntry(engine, list_->entries[index]);
	}
	return false;
}

QScriptValue BattleListEntries::newBattleListEntry(QScriptEngine* engine, const InternalBattleList::Entry& entry) {
	QScriptValue value = engine->newObject();
	value.setProperty("id", entry.id);
	value.setProperty("x", entry.position.x);
	value.setProperty("y", entry.position.y);
	value.setProperty("z", entry.position.z);
	value.setProperty("name", QString((char*) entry.name));
	return value;
}

QScriptValue BattleListEntries::findById(QScriptEngine* engine, quint32 id) {
	for(int index = 0; index < BATTLELIST_LENGTH; ++index) {
		if(list_->entries[index].id == id) {
			return newBattleListEntry(engine, index);
		}
	}
	return false;
}

QScriptValue BattleListEntries::findByName(QScriptEngine* engine, const QString& name) {
	QScriptValue array = engine->newArray();
	int arrayIndex = 0;
	for(int index = 0; index < BATTLELIST_LENGTH; ++index) {
		if(name.compare((char*) list_->entries[index].name) == 0) {
			array.setProperty(arrayIndex++, newBattleListEntry(engine, index));
		}
	}
	return array;
}

QScriptValue BattleListEntries::property(const QScriptValue& object, const QScriptString&, uint id) {
	/* The variable id contains the index that we want to access. */
	return newBattleListEntry(engine(), id);
}

BattleListEntries::QueryFlags BattleListEntries::queryProperty(const QScriptValue&, const QScriptString& name, QueryFlags flags, uint* id) {
	if(flags.testFlag(HandlesReadAccess) && name.isValid()) {
		bool success = false;
		quint32 index = name.toArrayIndex(&success);
		if(success) {
			/* Store the index in the in location where id points to. The "property" function doesn't have to do a new lookup then. */
			*id = index;
			return HandlesReadAccess;
		}
	}
	return 0;
}

QScriptClassPropertyIterator* BattleListEntries::newIterator(const QScriptValue& object) {
	return new BattleListEntriesIterator(object);
}

BattleListEntriesIterator::BattleListEntriesIterator(const QScriptValue& object): QScriptClassPropertyIterator(object) {
	toFront();
}

QScriptString BattleListEntriesIterator::name() const {
	return object().engine()->toStringHandle(QString::number(last_));
}

uint BattleListEntriesIterator::id() const {
	return last_;
}

bool BattleListEntriesIterator::hasNext() const {
	return index_ < BATTLELIST_LENGTH - 1;
}

bool BattleListEntriesIterator::hasPrevious() const {
	return index_ > 0;
}

void BattleListEntriesIterator::next() {
	last_ = index_;
	++index_;
}

void BattleListEntriesIterator::previous()  {
	--index_;
	last_ = index_;
}

void BattleListEntriesIterator::toBack() {
	index_ = BATTLELIST_LENGTH;
	last_ = -1;
}

void BattleListEntriesIterator::toFront() {
	index_ = 0;
	last_ = -1;
}
