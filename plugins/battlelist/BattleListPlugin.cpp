#include "BattleListPlugin.h"

EXPORT_PLUGIN(BattleListPlugin)

const QString BattleListPlugin::PLUGIN_NAME("battlelist");
const int BattleListPlugin::PLUGIN_VERSION(1);

const QString BattleListPlugin::VARIABLE_NAME("BattleList");

const InternalBattleList::List* BattleListPlugin::battleList = (InternalBattleList::List*) BATTLELIST_ADDRESS;

QString BattleListPlugin::name() const {
	return PLUGIN_NAME;
}

int BattleListPlugin::version() const {
	return PLUGIN_VERSION;
}

void BattleListPlugin::install(ScriptEngineInterface* engine) {
	engine_ = engine;
	entries_ = new BattleListEntries(engine_);
	QScriptValue object = engine_->newObject();
	object.setData(engine_->newQObject(this));
	object.setProperty("entries", engine_->newObject(entries_), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	object.setProperty("findById", engine_->newFunction(BattleListPlugin::findById), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	object.setProperty("findByName", engine_->newFunction(BattleListPlugin::findByName), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	engine_->globalObject().setProperty(VARIABLE_NAME, object, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void BattleListPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	delete entries_;
	entries_ = NULL;
	engine_ = NULL;
}

QScriptValue BattleListPlugin::newBattleListEntry(QScriptEngine* engine, quint32 index) {
	if(index < BATTLELIST_LENGTH) {
		return BattleListPlugin::newBattleListEntry(engine, battleList->entries[index]);
	}
	return false;
}

QScriptValue BattleListPlugin::newBattleListEntry(QScriptEngine* engine, const InternalBattleList::Entry& entry) {
	QScriptValue value = engine->newObject();
	value.setProperty("id", entry.id);
	value.setProperty("x", entry.position.x);
	value.setProperty("y", entry.position.y);
	value.setProperty("z", entry.position.z);
	value.setProperty("name", QString((char*) entry.name));
	return value;
}

QScriptValue BattleListPlugin::findById(QScriptEngine* engine, quint32 id) {
	for(int index = 0; index < BATTLELIST_LENGTH; ++index) {
		if(battleList->entries[index].id == id) {
			return BattleListPlugin::newBattleListEntry(engine, index);
		}
	}
	return false;
}

QScriptValue BattleListPlugin::findByName(QScriptEngine* engine, const QString& name) {
	QScriptValue array = engine->newArray();
	int arrayIndex = 0;
	for(int index = 0; index < BATTLELIST_LENGTH; ++index) {
		if(name.compare((char*) battleList->entries[index].name) == 0) {
			array.setProperty(arrayIndex++, BattleListPlugin::newBattleListEntry(engine, index));
		}
	}
	return array;
}

QScriptValue BattleListPlugin::findById(QScriptContext* context, QScriptEngine* engine) {
	if(context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if(value.isNumber()) {
			quint32 id = value.toUInt32();
			return BattleListPlugin::findById(engine, id);
		}
	}
	return context->throwError("invalid call to findById(Number)");
}

QScriptValue BattleListPlugin::findByName(QScriptContext* context, QScriptEngine* engine) {
	if(context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if(value.isString()) {
			QString name = value.toString();
			return BattleListPlugin::findByName(engine, name);
		}
	}
	return context->throwError("invalid call to findByName(String)");
}

QScriptValue BattleListEntries::property(const QScriptValue& object, const QScriptString&, uint id) {
	/* The variable id contains the index that we want to access. */
	return BattleListPlugin::newBattleListEntry(engine(), id);
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
