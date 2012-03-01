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

#include "MemoryPlugin.h"

EXPORT_PLUGIN(MemoryPlugin)

const QString MemoryPlugin::PLUGIN_NAME("memory");
const int MemoryPlugin::PLUGIN_VERSION(1);

const QString MemoryPlugin::VARIABLE_NAME("Memory");

QString MemoryPlugin::name() const {
	return PLUGIN_NAME;
}

int MemoryPlugin::version() const {
	return PLUGIN_VERSION;
}

void MemoryPlugin::install(ScriptEngineInterface* engine) {
	engine_ = engine;
	engine_->globalObject().setProperty(VARIABLE_NAME, engine->newQObject(this), QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void MemoryPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
}

quint8 MemoryPlugin::readU8(quint32 address) {
	quint8 value = *((quint8*) address);
	return value;
}

quint16 MemoryPlugin::readU16(quint32 address) {
	quint16 value = *((quint16*) address);
	return value;
}

quint32 MemoryPlugin::readU32(quint32 address) {
	quint32 value = *((quint32*) address);
	return value;
}

QString MemoryPlugin::readString(quint32 address) {
	return QString((const char*) address);
}
